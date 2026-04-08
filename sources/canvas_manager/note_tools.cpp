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
    auto &cursor{context_.interface.noteCanvas.cursorPosition.value()};

    if(cursor.isHoveringNote) return;

    const auto channelIndex{utilities::selectedInstrumentChannelIndex(context_.interface.sidebar.selectedChannelListViewIndex)};
    if(!channelIndex) return;

    actionCenter.addNote(
        context_.system.project.currentPage,
        channelIndex.value(),
        cursor.noteIndex,
        cursor.note
    );

}

void CanvasManager::handleNoteDeletion(ActionCenter &actionCenter){
    auto &cursor{context_.interface.noteCanvas.cursorPosition.value()};

    // if(!cursor.isHoveringNote) return;

    const auto channelIndex{utilities::selectedInstrumentChannelIndex(context_.interface.sidebar.selectedChannelListViewIndex)};
    if(!channelIndex) return;

    actionCenter.removeNote(
        context_.system.project.currentPage,
        channelIndex.value(),
        cursor.noteIndex
    );

}

void CanvasManager::handleInstrumentChange(ActionCenter &actionCenter){


}