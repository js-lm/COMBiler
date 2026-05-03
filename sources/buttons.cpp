#include "main_window.hpp"

#include "debug_utilities.hpp"

#include "constants.hpp"
#include "utilities/project_utilities.hpp"

#include <algorithm>

#include "serializer/serializer.hpp" 

void MainWindow::handlePageChangeButtonsEvents(){
    if(interfaceState_.navigationBar.isAddPageRequested){
        if(actionCenter_){
            actionCenter_->addPage(interfaceState_.navigationBar.requestedPageInsertionIndex);
            applyProjectTransientNavigationState();
        }

        interfaceState_.navigationBar.isAddPageRequested = false;
        interfaceState_.navigationBar.requestedPageInsertionIndex = constants::action_center::InvalidPageInsertionIndex;
        interfaceState_.noteCanvas.isGridLayoutDirty = true;
    }

    if(interfaceState_.navigationBar.isPageMoveRequested){
        if(actionCenter_){
            actionCenter_->movePage(
                interfaceState_.navigationBar.requestedPageMoveSourceIndex,
                interfaceState_.navigationBar.requestedPageMoveInsertionIndex
            );
            applyProjectTransientNavigationState();
        }

        interfaceState_.navigationBar.isPageMoveRequested = false;
        interfaceState_.navigationBar.requestedPageMoveSourceIndex = constants::action_center::InvalidPageInsertionIndex;
        interfaceState_.navigationBar.requestedPageMoveInsertionIndex = constants::action_center::InvalidPageInsertionIndex;
        interfaceState_.noteCanvas.isGridLayoutDirty = true;
    }

    const auto projectData{utilities::projectDataWithPagesFrom(systemState_)};
    if(!projectData) return;

    const int maximumPageNumber{std::max(1, static_cast<int>(projectData->pages.size()))};

    if(interfaceState_.navigationBar.requestedPageNumber > 0){
        systemState_.project.currentPage = std::clamp(
            interfaceState_.navigationBar.requestedPageNumber,
            1, maximumPageNumber
        );
        interfaceState_.navigationBar.requestedPageNumber = 0;
        interfaceState_.noteCanvas.isGridLayoutDirty = true;
    }

    if(interfaceState_.navigationBar.isPreviousPageButtonPressed){
        systemState_.project.currentPage = std::max(1, systemState_.project.currentPage - 1);
        interfaceState_.navigationBar.isPreviousPageButtonPressed = false;
        interfaceState_.noteCanvas.isGridLayoutDirty = true;
    }


    if(interfaceState_.navigationBar.isNextPageButtonPressed){
        systemState_.project.currentPage = std::min(maximumPageNumber, systemState_.project.currentPage + 1);
        interfaceState_.navigationBar.isNextPageButtonPressed = false;
        interfaceState_.noteCanvas.isGridLayoutDirty = true;
    }

    // DEBUG_PRINT_IF_CHANGED("maximumPageNumber: {}", maximumPageNumber);

}

void MainWindow::handleToolbarButtonsEvents(){
    if(interfaceState_.toolbar.isUndoButtonPressed && actionCenter_ && actionCenter_->undo()){
        applyProjectTransientNavigationState();
    }
    if(interfaceState_.toolbar.isRedoButtonPressed && actionCenter_ && actionCenter_->redo()){
        applyProjectTransientNavigationState();
    }

    if(interfaceState_.toolbar.isConstantsButtonPressed){
        interfaceState_.prompts.isConstantsManagerWindowVisible = true;
        interfaceState_.prompts.isCommandWindowVisible = false;
    }

    if(auto projectData{systemState_.project.data.lock()}){
        // DEBUG_PRINT("{}", Serializer::toString(*projectData.d));
        if(interfaceState_.toolbar.isSaveFileButtonPressed) serializer_->save(*projectData->data);
        if(interfaceState_.toolbar.isSaveAsFileButtonPressed) serializer_->save(*projectData->data, true);

        if(interfaceState_.toolbar.isLoadFileButtonPressed){

            projectData->data.reset();

            if(auto loadedData{serializer_->load()}){
                actionCenter_->loadFile(loadedData.value());
            }
        }

        if(const auto filename{serializer_->getCurrentFilename()}; !filename.empty()){
            std::string newWindowTitle{
                constants::application_window::Title 
              + std::string{" - "} + filename
            };
            SetWindowTitle(newWindowTitle.c_str());
        }
    }


    const bool isSystemChannelSelected{
        interfaceState_.sidebar.selectedChannelListViewIndex == constants::interface_layout::note_canvas::notes::SystemChannelListViewIndex
    };

    if(isSystemChannelSelected){
        if(IsKeyPressed(KEY_Q)) interfaceState_.prompts.selectedCommandTool = constants::prompts::CommandPrompt::Tempo;
        if(IsKeyPressed(KEY_W)) interfaceState_.prompts.selectedCommandTool = constants::prompts::CommandPrompt::Volume;
        if(IsKeyPressed(KEY_E)) interfaceState_.prompts.selectedCommandTool = constants::prompts::CommandPrompt::Articulation;
        if(IsKeyPressed(KEY_R)){
            interfaceState_.prompts.activeCommandPrompt = interfaceState_.prompts.selectedCommandTool;
            interfaceState_.prompts.isCommandWindowVisible = true;
        }
    }else{
        if(IsKeyPressed(KEY_Q)) interfaceState_.toolbar.selectedTool = constants::toolbar::Tool::Cursor;
        if(IsKeyPressed(KEY_W)) interfaceState_.toolbar.selectedTool = constants::toolbar::Tool::Pen;
        if(IsKeyPressed(KEY_E)) interfaceState_.toolbar.selectedTool = constants::toolbar::Tool::Eraser;
        if(IsKeyPressed(KEY_R)) interfaceState_.toolbar.selectedTool = constants::toolbar::Tool::Change_Instrument;
    }

    const int previousChannelIndex{interfaceState_.sidebar.selectedChannelListViewIndex};

    if(IsKeyPressed(KEY_GRAVE)) interfaceState_.sidebar.selectedChannelListViewIndex = constants::sidebar::AllChannelsListViewIndex;
    if(IsKeyPressed(KEY_ONE)) interfaceState_.sidebar.selectedChannelListViewIndex = constants::sidebar::FirstInstrumentChannelListViewIndex;
    if(IsKeyPressed(KEY_TWO)) interfaceState_.sidebar.selectedChannelListViewIndex = constants::sidebar::SecondInstrumentChannelListViewIndex;
    if(IsKeyPressed(KEY_THREE)) interfaceState_.sidebar.selectedChannelListViewIndex = constants::sidebar::ThirdInstrumentChannelListViewIndex;
    if(IsKeyPressed(KEY_FOUR)) interfaceState_.sidebar.selectedChannelListViewIndex = constants::sidebar::FourthInstrumentChannelListViewIndex;
    if(IsKeyPressed(KEY_FIVE)) interfaceState_.sidebar.selectedChannelListViewIndex = constants::sidebar::SystemChannelListViewIndex;

    // TODO: duplicated, but I don't really have any other good way to deal with keyboard shortcuts (inspector.cpp: void Sidebar::drawInspector())
    if(previousChannelIndex != interfaceState_.sidebar.selectedChannelListViewIndex){
        if(interfaceState_.prompts.activeCommandPrompt == constants::prompts::CommandPrompt::Constant){
            interfaceState_.prompts.activeCommandPrompt = constants::prompts::CommandPrompt::Tempo;
            interfaceState_.prompts.selectedCommandTool = constants::prompts::CommandPrompt::Tempo;
        }
    }
}