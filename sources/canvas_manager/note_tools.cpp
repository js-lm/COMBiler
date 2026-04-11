#include "canvas_manager.hpp"

#include <raylib.h>

#include "constants.hpp"
#include "utilities/project_utilities.hpp"

#include <algorithm>
#include <type_traits>

#include "debug_utilities.hpp"

void CanvasManager::handleNoteTools(ActionCenter &actionCenter){

    handleSelectAll();
    handleCopyAndPasteModeState();

    if(context_.interface.prompts.isCommandWindowVisible) return;

    if(context_.interface.clipboard.isPasteModeEnabled) return;

    const bool isLeftMouseButtonDown{IsMouseButtonDown(MOUSE_BUTTON_LEFT)};

    if(!context_.interface.noteCanvas.cursorPosition){
        if(!isLeftMouseButtonDown){
            isSelectionDragInProgress_ = false;

            if(hasActionStarted_){
                actionCenter.finishAction();
                hasActionStarted_ = false;
            }
        }
        return;
    }

    if(handleCommandTools(actionCenter)) return;

    if(isLeftMouseButtonDown){
        switch(context_.interface.toolbar.selectedTool){
        case constants::toolbar::Tool::Cursor:  handleSelection(actionCenter); break;
        case constants::toolbar::Tool::Pen:     handleNoteAdding(actionCenter); break;
        case constants::toolbar::Tool::Eraser:  handleNoteDeletion(actionCenter); break;
        case constants::toolbar::Tool::Change_Instrument: handleInstrumentChange(actionCenter); break;
        }

    }else{
        isSelectionDragInProgress_ = false;

        if(hasActionStarted_){
            actionCenter.finishAction();
            hasActionStarted_ = false;
        }
    }

}

bool CanvasManager::hasSelectionArea() const{
    return context_.interface.clipboard.selectionArea.has_value()
        && context_.interface.clipboard.selectionArea.value().widthInCells > 0
        && context_.interface.clipboard.selectionArea.value().heightInCells > 0;
}

bool CanvasManager::isCommandChannelSelected() const{
    return context_.interface.sidebar.selectedChannelListViewIndex == constants::sidebar::SystemChannelListViewIndex;
}

void CanvasManager::handleCopyAndPasteModeState(){
    auto &toolbarState{context_.interface.toolbar};
    auto &clipboardState{context_.interface.clipboard};

    const bool isControlDown{IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)};
    const bool isCopyShortcutPressed{isControlDown && IsKeyPressed(KEY_C)};

    const bool isCopyRequested{toolbarState.isCopyNoteButtonPressed || isCopyShortcutPressed};
    toolbarState.isCopyNoteButtonPressed = false;
    toolbarState.isPasteNoteButtonPressed = false;

    if(isCommandChannelSelected()){
        clipboardState.isPasteModeEnabled = false;
        return;
    }

    if(isCopyRequested && hasSelectionArea()){
        const auto projectData{utilities::projectDataWithPagesFrom(context_.system)};
        if(!projectData) return;

        const auto &selectionArea{clipboardState.selectionArea.value()};
        const auto &currentPage{utilities::pageByNumber(*projectData, context_.system.project.currentPage)};

        clipboardState.copiedFromAllChannels = context_.interface.sidebar.selectedChannelListViewIndex == constants::sidebar::AllChannelsListViewIndex;
        clipboardState.copiedInstrumentChannelIndex = -1; // TODO: magic numbers

        if(!clipboardState.copiedFromAllChannels){

            const auto selectedInstrumentChannel{selectedInstrumentChannelIndex()};

            if(!selectedInstrumentChannel.has_value()) return;

            clipboardState.copiedInstrumentChannelIndex = selectedInstrumentChannel.value();

        }

        for(auto &instrumentChannel: clipboardState.instrumentChannels){
            instrumentChannel.assign(selectionArea.widthInCells, std::nullopt);
        }
        clipboardState.commandChannel.assign(selectionArea.widthInCells, std::nullopt);

        auto rowIndexFromNote{[](music_data::Note note){

            return constants::interface_layout::note_canvas::FirstNoteOffsetFromC0
                 + (constants::interface_layout::note_canvas::NumberOfRow - 1)
                 - static_cast<int>(note);
        }};

        auto shouldCopyCell{[&](const std::optional<music_data::InstrumentChannelData> &cell){
            if(!cell.has_value()) return false;
            if(std::holds_alternative<music_data::Instrument>(cell.value())) return true;

            const int rowIndex{rowIndexFromNote(std::get<music_data::Note>(cell.value()))};
            return rowIndex >= selectionArea.topLeftRowIndex
                && rowIndex < selectionArea.topLeftRowIndex + selectionArea.heightInCells;

        }};

        auto commandAffectsChannel{[](const command::CommandToken &commandToken, int instrumentChannelIndex){
            if(!std::holds_alternative<command::Command>(commandToken)) return false;

            const auto commandData{std::get<command::Command>(commandToken)};
            const auto targetForChannel{static_cast<command::Target>(instrumentChannelIndex + 1)};

            return std::visit([targetForChannel](const auto &commandVariant){
                using Type = std::decay_t<decltype(commandVariant)>;

                if constexpr(std::is_same_v<Type, command::Tempo>){

                    return true;
                }else{

                    return commandVariant.target == command::Target::All_Channels
                        || commandVariant.target == targetForChannel;
                }

            }, commandData);
        }};

        for(int selectionColumnOffset{0}; selectionColumnOffset < selectionArea.widthInCells; selectionColumnOffset++){
            const int sourceColumnIndex{selectionArea.topLeftColumnIndex + selectionColumnOffset};
            if(sourceColumnIndex < 0 || sourceColumnIndex >= constants::project_data::MaximumNotePerPage) continue;

            if(clipboardState.copiedFromAllChannels){
                for(int instrumentChannelIndex{0}; instrumentChannelIndex < constants::project_data::NumberOfInstrumentChannels; instrumentChannelIndex++){
                    const auto &cell{currentPage.instrumentChannels[instrumentChannelIndex][sourceColumnIndex]};
                    
                    if(!shouldCopyCell(cell)) continue;

                    clipboardState.instrumentChannels[instrumentChannelIndex][selectionColumnOffset] = cell;

                }
            }else{
                const int instrumentChannelIndex{clipboardState.copiedInstrumentChannelIndex};
                const auto &cell{currentPage.instrumentChannels[instrumentChannelIndex][sourceColumnIndex]};

                if(shouldCopyCell(cell)){
                    clipboardState.instrumentChannels[instrumentChannelIndex][selectionColumnOffset] = cell;
                }

            }

            if(clipboardState.copiedFromAllChannels){
                clipboardState.commandChannel[selectionColumnOffset] = currentPage.commandChannel[sourceColumnIndex];
            }else if(currentPage.commandChannel[sourceColumnIndex].has_value()){
                const auto &commandToken{currentPage.commandChannel[sourceColumnIndex].value()};
                const int instrumentChannelIndex{clipboardState.copiedInstrumentChannelIndex};

                if(commandAffectsChannel(commandToken, instrumentChannelIndex)){
                    clipboardState.commandChannel[selectionColumnOffset] = commandToken;
                }
                
            }
        }

        clipboardState.hasCopiedSelection = true;
        clipboardState.isPasteModeEnabled = true;
        clipboardState.copiedWidthInCells = selectionArea.widthInCells;
        clipboardState.copiedHeightInCells = selectionArea.heightInCells;
        clipboardState.copiedCenterRowIndex = selectionArea.topLeftRowIndex + (selectionArea.heightInCells / 2);

        // DEBUG_PRINT(
        //     "copy selection -> width:{}, height:{}, fromAllChannels:{}, instrumentChannelIndex:{}",
        //     clipboardState.copiedWidthInCells,
        //     clipboardState.copiedHeightInCells,
        //     clipboardState.copiedFromAllChannels,
        //     clipboardState.copiedInstrumentChannelIndex
        // );

        context_.interface.toolbar.selectedTool = constants::toolbar::Tool::Cursor;
        isSelectionDragInProgress_ = false;
        return;
    }

    // DEBUG_PRINT_IF_CHANGED("isPasteModeEnabled: {}", clipboardState.isPasteModeEnabled);
}

void CanvasManager::handleSelection(ActionCenter &actionCenter){
    const auto &cursor{cursorPosition()};

    const int currentColumnIndex{cursor.noteIndex};
    const int currentRowIndex{
        constants::interface_layout::note_canvas::FirstNoteOffsetFromC0
      + (constants::interface_layout::note_canvas::NumberOfRow - 1)
      - static_cast<int>(cursor.note)
    };

    if(!isSelectionDragInProgress_){
        selectionDragStartColumnIndex_ = currentColumnIndex;
        selectionDragStartRowIndex_ = currentRowIndex;
        isSelectionDragInProgress_ = true;
    }

    const int leftColumnIndex{std::min(selectionDragStartColumnIndex_, currentColumnIndex)};
    const int rightColumnIndex{std::max(selectionDragStartColumnIndex_, currentColumnIndex)};

    const int topRowIndex{std::min(selectionDragStartRowIndex_, currentRowIndex)};
    const int bottomRowIndex{std::max(selectionDragStartRowIndex_, currentRowIndex)};

    const int widthInCells{(rightColumnIndex - leftColumnIndex) + 1};
    const int heightInCells{(bottomRowIndex - topRowIndex) + 1};

    // DEBUG_PRINT_IF_CHANGED(
    //     "leftColumnIndex:{}, topRowIndex: {}. widthInCells: {}, heightInCells: {}",
    //     leftColumnIndex, topRowIndex, widthInCells, heightInCells
    // );

    if(widthInCells == 1 && heightInCells == 1){
        context_.interface.clipboard.selectionArea = std::nullopt;
        return;
    }

    context_.interface.clipboard.selectionArea = program_states::Interface::Clipboard::SelectionArea{
        .topLeftColumnIndex{leftColumnIndex},
        .topLeftRowIndex{topRowIndex},
        .widthInCells{widthInCells},
        .heightInCells{heightInCells}
    };
}

void CanvasManager::handleNoteAdding(ActionCenter &actionCenter){
    const auto &cursor{cursorPosition()};

    if(cursor.isHoveringNote) return;

    hasActionStarted_ = true;

    const auto instrumentChannelIndex{selectedInstrumentChannelIndex()};
    if(!instrumentChannelIndex) return;

    actionCenter.addNote(
        context_.system.project.currentPage,
        instrumentChannelIndex.value(),
        cursor.noteIndex,
        cursor.note
    );

}

void CanvasManager::handleNoteDeletion(ActionCenter &actionCenter){
    const auto &cursor{cursorPosition()};

    // if(!cursor.isHoveringNote) return;

    const auto instrumentChannelIndex{selectedInstrumentChannelIndex()};
    if(!instrumentChannelIndex) return;

    hasActionStarted_ = true;

    actionCenter.removeNote(
        context_.system.project.currentPage,
        instrumentChannelIndex.value(),
        cursor.noteIndex
    );

}

void CanvasManager::handleInstrumentChange(ActionCenter &actionCenter){
    const auto &cursor{cursorPosition()};

    const auto instrumentChannelIndex{selectedInstrumentChannelIndex()};
    if(!instrumentChannelIndex) return;

    hasActionStarted_ = true;

    actionCenter.addInstrumentChange(
        context_.system.project.currentPage,
        instrumentChannelIndex.value(),
        cursor.noteIndex,
        context_.interface.prompts.selectedInstrument
    );

}

void CanvasManager::handleSelectAll(){
	const bool isControlDown{IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)};
	if(!isControlDown || !IsKeyPressed(KEY_A)) return;

    if(context_.interface.sidebar.selectedChannelListViewIndex == constants::interface_layout::note_canvas::notes::SystemChannelListViewIndex) return;

	const auto projectData{utilities::projectDataWithPagesFrom(context_.system)};
	if(!projectData) return;

    const int currentPageNoteCount{utilities::currentPageNoteCountFrom(*projectData, context_.system.project.currentPage)};
    
    context_.interface.toolbar.selectedTool = constants::toolbar::Tool::Cursor;

    isSelectionDragInProgress_ = false;

	context_.interface.clipboard.selectionArea = program_states::Interface::Clipboard::SelectionArea{
		.topLeftColumnIndex{0},
		.topLeftRowIndex{0},
        .widthInCells{currentPageNoteCount},
		.heightInCells{constants::interface_layout::note_canvas::NumberOfRow}
	};
}