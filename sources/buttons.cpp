#include "main_window.hpp"

#include "debug_utilities.hpp"

#include "constants.hpp"
#include "utilities/project_utilities.hpp"

#include <algorithm>

void MainWindow::handlePageChangeButtonsEvents(){
    const auto projectData{utilities::projectDataWithPagesFrom(systemState_)};
    if(!projectData) return;

    const int maximumPageNumber{std::max(1, static_cast<int>(projectData->pages.size()))};

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

    DEBUG_PRINT_IF_CHANGED("maximumPageNumber: {}", maximumPageNumber);

}

void MainWindow::handleToolbarButtonsEvents(){
    if(interfaceState_.toolbar.isUndoButtonPressed && actionCenter_ && actionCenter_->undo()){
        applyProjectTransientNavigationState();
    }
    if(interfaceState_.toolbar.isRedoButtonPressed && actionCenter_ && actionCenter_->redo()){
        applyProjectTransientNavigationState();
    }

    // TODO: magic numbers
    if(IsKeyPressed(KEY_Q)) interfaceState_.toolbar.selectedToolIndex = 0;
    if(IsKeyPressed(KEY_W)) interfaceState_.toolbar.selectedToolIndex = 1;
    if(IsKeyPressed(KEY_E)) interfaceState_.toolbar.selectedToolIndex = 2;
    if(IsKeyPressed(KEY_R)) interfaceState_.toolbar.selectedToolIndex = 3;

    if(IsKeyPressed(KEY_GRAVE)) interfaceState_.sidebar.selectedChannelListViewIndex = 0;
    if(IsKeyPressed(KEY_ONE)) interfaceState_.sidebar.selectedChannelListViewIndex = 1;
    if(IsKeyPressed(KEY_TWO)) interfaceState_.sidebar.selectedChannelListViewIndex = 2;
    if(IsKeyPressed(KEY_THREE)) interfaceState_.sidebar.selectedChannelListViewIndex = 3;
    if(IsKeyPressed(KEY_FOUR)) interfaceState_.sidebar.selectedChannelListViewIndex = 4;
    if(IsKeyPressed(KEY_FIVE)) interfaceState_.sidebar.selectedChannelListViewIndex = 5;
}