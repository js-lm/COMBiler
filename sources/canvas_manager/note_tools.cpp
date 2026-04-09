#include "canvas_manager.hpp"

#include <raylib.h>

#include "constants.hpp"
#include "utilities/project_utilities.hpp"

void CanvasManager::handleNoteTools(ActionCenter &actionCenter){

    if(context_.interface.prompts.isCommandWindowVisible) return;

    if(!context_.interface.noteCanvas.cursorPosition) return;

    if(handleCommandTools(actionCenter)) return;

    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
        hasActionStarted_ = true;

        switch(context_.interface.toolbar.selectedTool){
        case constants::toolbar::Tool::Cursor:  handleSelection(actionCenter); break;
        case constants::toolbar::Tool::Pen:     handleNoteAdding(actionCenter); break;
        case constants::toolbar::Tool::Eraser:  handleNoteDeletion(actionCenter); break;
        case constants::toolbar::Tool::Change_Instrument: handleInstrumentChange(actionCenter); break;
        }

    }else{
        if(hasActionStarted_){
            actionCenter.finishAction();
            hasActionStarted_ = false;
        }
    }

}

void CanvasManager::handleSelection(ActionCenter &actionCenter){


}

void CanvasManager::handleNoteAdding(ActionCenter &actionCenter){
    const auto &cursor{cursorPosition()};

    if(cursor.isHoveringNote) return;

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

    actionCenter.addInstrumentChange(
        context_.system.project.currentPage,
        instrumentChannelIndex.value(),
        cursor.noteIndex,
        context_.interface.prompts.selectedInstrument
    );

}

const program_states::Interface::NoteCanvas::CursorPosition &CanvasManager::cursorPosition() const{
    return context_.interface.noteCanvas.cursorPosition.value();
}

std::optional<int> CanvasManager::selectedInstrumentChannelIndex() const{
    return utilities::selectedInstrumentChannelIndex(context_.interface.sidebar.selectedChannelListViewIndex);
}
