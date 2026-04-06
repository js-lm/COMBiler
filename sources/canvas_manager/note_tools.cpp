#include "canvas_manager.hpp"

#include <raylib.h>

#include "constants.hpp"

void CanvasManager::handleNoteTools(ActionCenter &actionCenter){

    if(!context_.interface.noteCanvas.cursorPosition) return;
    

    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
        hasActionStarted_ = true;

        switch(context_.interface.toolbar.selectedToolIndex){
        // TODO: magic numbers
        case 0:
            break;

        case 1: handleNoteAdding(actionCenter); break;
            
        case 2:
            break;

        case 3:
            break;
        
        }

    }else{
        if(hasActionStarted_){
            actionCenter.finishAction();
            hasActionStarted_ = false;
        }



    }


}

void CanvasManager::handleNoteAdding(ActionCenter &actionCenter){
    auto &cursor{context_.interface.noteCanvas.cursorPosition.value()};

    if(cursor.isHoveringNote) return;

    const int selectedChannelListIndex{context_.interface.sidebar.selectedChannelListViewIndex};
    const bool hasSpecificChannelSelection{ // TODO: this should be a member function? too many duplications
        selectedChannelListIndex >= 1
     && selectedChannelListIndex <= constants::project_data::NumberOfInstrumentChannels
    };
    if(!hasSpecificChannelSelection) return;

    const int channelIndex{selectedChannelListIndex - 1};

    actionCenter.addNote(
        context_.system.project.currentPage,
        channelIndex,
        cursor.noteIndex,
        cursor.note
    );

}

void CanvasManager::handleNoteDeletion(ActionCenter &actionCenter){
    auto &cursor{context_.interface.noteCanvas.cursorPosition.value()};

    if(!cursor.isHoveringNote) return;





}