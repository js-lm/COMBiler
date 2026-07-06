#include "canvas_manager.hpp"
#include "utilities/project_utilities.hpp"

void CanvasManager::update(ActionCenter &actionCenter, MidiManager &midiManager){
    if(context_.interface.navigationBar.isTimelineDraggingPage) return;
    if(context_.interface.navigationBar.isPageSelectEnabled) return;

    if(!hasInitializedSelectionTracking_){
        previousSelectedTool_ = context_.interface.toolbar.selectedTool;
        previousSelectedChannelListViewIndex_ = context_.interface.sidebar.selectedChannelListViewIndex;
        hasInitializedSelectionTracking_ = true;
    }

    const bool hasToolSwitched{context_.interface.toolbar.selectedTool != previousSelectedTool_};
    const bool hasChannelSwitched{
        context_.interface.sidebar.selectedChannelListViewIndex != previousSelectedChannelListViewIndex_
    };

    if(hasToolSwitched || hasChannelSwitched) clearSelectionState();

    handleNoteTools(actionCenter, midiManager);

    previousSelectedTool_ = context_.interface.toolbar.selectedTool;
    previousSelectedChannelListViewIndex_ = context_.interface.sidebar.selectedChannelListViewIndex;
}