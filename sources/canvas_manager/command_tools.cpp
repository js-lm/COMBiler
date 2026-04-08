#include "canvas_manager.hpp"

#include <raylib.h>

#include "constants.hpp"
#include "utilities/project_utilities.hpp"

bool CanvasManager::handleCommandTools(ActionCenter &actionCenter){
    if(context_.interface.sidebar.selectedChannelListViewIndex != constants::sidebar::SystemChannelListViewIndex) return false;

    // TODO: check if there already exist the same command

    auto &cursor{context_.interface.noteCanvas.cursorPosition.value()};

    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
        hasActionStarted_ = true;
        actionCenter.addCommand(
            context_.system.project.currentPage,
            cursor.noteIndex,
            context_.interface.prompts
        );
    }else if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
        hasActionStarted_ = true;
        actionCenter.removeCommand(
            context_.system.project.currentPage,
            cursor.noteIndex
        );

    }else if(hasActionStarted_){
        actionCenter.finishAction();
        hasActionStarted_ = false;
    }

    return true;
    

}