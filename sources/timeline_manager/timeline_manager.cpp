#include "timeline_manager.hpp"

void TimelineManager::update(ActionCenter &actionCenter){

    
    
}

void TimelineManager::cancelActiveInteraction(){
	interactionMode_ = InteractionMode::None;
	dragAndDropState_.isDraggingPage = false;
	selectionAndPasteState_.isPasteModeEnabled = false;
}