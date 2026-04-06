#include "canvas_manager.hpp"


void CanvasManager::update(ActionCenter &actionCenter){
    handleNoteTools(actionCenter);
    handlePageTools(actionCenter);
}