#include "canvas_manager.hpp"

#include <raylib.h>

#include "constants.hpp"
#include "utilities/project_utilities.hpp"

#include <algorithm>

#include "debug_utilities.hpp"

void CanvasManager::handleNoteTools(ActionCenter &actionCenter){

    handleSelectAll();

    if(context_.interface.prompts.isCommandWindowVisible) return;

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

    DEBUG_PRINT_IF_CHANGED(
        "leftColumnIndex:{}, topRowIndex: {}. widthInCells: {}, heightInCells: {}",
        leftColumnIndex, topRowIndex, widthInCells, heightInCells
    );

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