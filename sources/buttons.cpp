#include "main_window.hpp"

#include "debug_utilities.hpp"

#include "constants.hpp"

#include <algorithm>

void MainWindow::handlePageChangeButtonsEvents(){
    const auto projectDataSlot{systemState_.project.data.lock()};
    if(!projectDataSlot || !projectDataSlot->data || projectDataSlot->data->pages.empty()) return;
    
	const auto &projectData{projectDataSlot->data};

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
}