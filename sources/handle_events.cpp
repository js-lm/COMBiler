#include "main_window.hpp"

#include "constants.hpp"

#include <algorithm>
#include <cmath>

#include "debug_utilities.hpp"
#include "utilities/project_utilities.hpp"

void MainWindow::handleEvents(){
    
    handleKeyboardEvent();
    handleButtonPressEvents();

    if(IsWindowResized()) handleWindowSizeChangeEvent();

    if(IsFileDropped()){
        FilePathList droppedFiles{LoadDroppedFiles()};
        if(droppedFiles.count > 0){
            if(actionCenter_->isUnsaved()){
                interfaceState_.prompts.droppedFilePath = std::string{droppedFiles.paths[0]};
                interfaceState_.prompts.isOverwritePromptVisible = true;
                interfaceState_.prompts.overwriteAction = program_states::Interface::Prompts::OverwriteAction::LoadDroppedFile;
            }else{
                interfaceState_.prompts.droppedFilePath = std::string{droppedFiles.paths[0]};
                interfaceState_.prompts.isOverwriteConfirmed = true;
                interfaceState_.prompts.overwriteAction = program_states::Interface::Prompts::OverwriteAction::LoadDroppedFile;
            }
        }
        UnloadDroppedFiles(droppedFiles);
    }

    if(interfaceState_.prompts.isOverwriteConfirmed){
        if(interfaceState_.prompts.overwriteAction == program_states::Interface::Prompts::OverwriteAction::LoadDroppedFile){
            if(auto loadedData{serializer_->load(interfaceState_.prompts.droppedFilePath)}){
                if(auto projectData{systemState_.project.data.lock()}){
                    projectData->data.reset();
                }
                loadProjectData(loadedData.value());
            }
        }else if(interfaceState_.prompts.overwriteAction == program_states::Interface::Prompts::OverwriteAction::LoadFile){
            if(auto loadedData{serializer_->load()}){
                if(auto projectData{systemState_.project.data.lock()}){
                    projectData->data.reset();
                }
                loadProjectData(loadedData.value());
            }
        }else if(interfaceState_.prompts.overwriteAction == program_states::Interface::Prompts::OverwriteAction::NewFile){
            program_states::ProjectData freshProjectData{};
            loadProjectData(freshProjectData);
        }

        interfaceState_.prompts.isOverwriteConfirmed = false;
        interfaceState_.prompts.overwriteAction = program_states::Interface::Prompts::OverwriteAction::None;
        interfaceState_.prompts.droppedFilePath = "";
    }

    handleConstantsManagerEvents();

    if(interfaceState_.prompts.hasModifiedMetadata){
        actionCenter_->markAsUnsaved();
        interfaceState_.prompts.hasModifiedMetadata = false;
    }
}

void MainWindow::handleButtonPressEvents(){

    handleToolbarButtonsEvents();
    handlePageChangeButtonsEvents();

}

void MainWindow::handleWindowSizeChangeEvent(){
    using namespace constants::interface_layout;

    auto &window{systemState_.window};
    const int screenWidth{GetScreenWidth()};
    const int screenHeight{GetScreenHeight()};

    window.scaleFactor = calculateScaleFactor();

    SetMouseScale(1.0f / window.scaleFactor, 1.0f / window.scaleFactor);

    const int scaledRenderWidth{std::max(1,
        window.isIntegerScaling
            ? static_cast<int>(std::floor(static_cast<float>(screenWidth) / window.scaleFactor))
            : static_cast<int>(std::round(static_cast<float>(screenWidth) / window.scaleFactor))
    )};
    const int scaledRenderHeight{std::max(1,
        window.isIntegerScaling
            ? static_cast<int>(std::floor(static_cast<float>(screenHeight) / window.scaleFactor))
            : static_cast<int>(std::round(static_cast<float>(screenHeight) / window.scaleFactor))
    )};

    window.interfaceRenderTextureWidth  = scaledRenderWidth;
    window.interfaceRenderTextureHeight = scaledRenderHeight;

    if(interfaceRenderTexture_.id != 0) UnloadRenderTexture(interfaceRenderTexture_);

    interfaceRenderTexture_ = LoadRenderTexture(
        window.interfaceRenderTextureWidth, 
        window.interfaceRenderTextureHeight
    );

    applyTextureFilter();


    auto &anchor{layoutState_.anchor};
    auto &bounds{layoutState_.bounds};


    /* navigation bar */ {
        anchor.navigationBar.transportControls.y = window.interfaceRenderTextureHeight - NavigationRowVerticalOffset;
        anchor.navigationBar.perPageSettings.y = window.interfaceRenderTextureHeight - NavigationRowVerticalOffset;
        anchor.navigationBar.status.y = window.interfaceRenderTextureHeight - NavigationRowVerticalOffset;

        anchor.navigationBar.timeline.y = window.interfaceRenderTextureHeight - TimelineVerticalOffset;


        bounds.navigationBar.timeline.groupBox.width = window.interfaceRenderTextureWidth;

        bounds.navigationBar.timeline.scrollPanel.width = bounds.navigationBar.timeline.groupBox.width - bounds.navigationBar.timeline.scrollPanel.x - TimelineScrollPanelRightPadding;


        bounds.navigationBar.timeline.pageNumberLabel.x = bounds.navigationBar.timeline.groupBox.width - TimelinePageNumberLabelOffset;
        bounds.navigationBar.timeline.pageNumberValueLabel.x = bounds.navigationBar.timeline.groupBox.width - TimelinePageNumberValueLabelOffset;
        bounds.navigationBar.timeline.addPageButton.x = bounds.navigationBar.timeline.groupBox.width - TimelineAddPageButtonOffset;

        bounds.navigationBar.timeline.pageCopyButton.x = bounds.navigationBar.timeline.groupBox.width - TimelinePageCopyButtonOffset;
        bounds.navigationBar.timeline.pagePasteButton.x = bounds.navigationBar.timeline.groupBox.width - TimelinePagePasteButtonOffset;
        bounds.navigationBar.timeline.pageCutButton.x = bounds.navigationBar.timeline.groupBox.width - TimelinePageCutButtonOffset;
        bounds.navigationBar.timeline.pageSelectToggle.x = bounds.navigationBar.timeline.groupBox.width - TimelinePageSelectToggleOffset;

        const float navigationBarSectionWidth{(window.interfaceRenderTextureWidth - NavigationContentLeftEdgeX) / static_cast<float>(NavigationSectionCount)};

        anchor.navigationBar.status.x = NavigationContentLeftEdgeX;
        anchor.navigationBar.transportControls.x = NavigationContentLeftEdgeX + navigationBarSectionWidth;
        anchor.navigationBar.perPageSettings.x = NavigationContentLeftEdgeX + (navigationBarSectionWidth * 2.0f);

        bounds.navigationBar.status.groupBox.width = navigationBarSectionWidth;
        bounds.navigationBar.transportControls.groupBox.width = navigationBarSectionWidth;
        bounds.navigationBar.perPageSettings.groupBox.width = navigationBarSectionWidth;

        const float transportControlsButtonsStartX{(bounds.navigationBar.transportControls.groupBox.width - TransportControlsButtonsTotalWidth) / 2.0f};

        bounds.navigationBar.transportControls.previousPageButton.x = transportControlsButtonsStartX;
        bounds.navigationBar.transportControls.nextPageButton.x = transportControlsButtonsStartX + TransportControlsNextPageButtonOffset;
        bounds.navigationBar.transportControls.playButton.x = transportControlsButtonsStartX + TransportControlsPlayButtonOffset;
        bounds.navigationBar.transportControls.stopButton.x = transportControlsButtonsStartX + TransportControlsStopButtonOffset;
        bounds.navigationBar.transportControls.pageRepeatToggle.x = transportControlsButtonsStartX + TransportControlsPageRepeatToggleOffset;

        bounds.navigationBar.perPageSettings.notePerPageSpinner.x = bounds.navigationBar.perPageSettings.groupBox.width - PerPageSpinnerRightPadding;
    } /* navigation bar */



    /* sidebar */ {
        bounds.sidebar.inspector.groupBox.height = window.interfaceRenderTextureHeight - layoutState_.anchor.sidebar.inspect.y - TimelineVerticalOffset;
    } /* sidebar */


    /* toolbar */ {

        bounds.toolbar.toolOption.groupBox.width = window.interfaceRenderTextureWidth - anchor.toolbar.toolOption.x;
        bounds.toolbar.toolOption.infoButton.x = bounds.toolbar.toolOption.groupBox.width - bounds.toolbar.toolOption.infoButton.width - ToolOptionInfoButtonRightPadding;
    } /* toolbar */

    /* prompts */ {
        auto centeredPromptAnchorFrom{[&](const Rectangle &promptWindowBounds){
            return Vector2{
                (static_cast<float>(window.interfaceRenderTextureWidth) - promptWindowBounds.width) * .5f,
                (static_cast<float>(window.interfaceRenderTextureHeight) - promptWindowBounds.height) * .5f
            };
        }};

        anchor.prompts.tempoWindow = centeredPromptAnchorFrom(bounds.prompts.tempo.windowBox);
        anchor.prompts.volumeWindow = centeredPromptAnchorFrom(bounds.prompts.volume.windowBox);
        anchor.prompts.articulationWindow = centeredPromptAnchorFrom(bounds.prompts.articulation.windowBox);
        anchor.prompts.instrumentWindow = centeredPromptAnchorFrom(bounds.prompts.instrument.windowBox);
        
        anchor.prompts.constantsManagerWindow = centeredPromptAnchorFrom(bounds.prompts.constantsManager.windowBox);
        anchor.prompts.constantsManagerWarningWindow = centeredPromptAnchorFrom(bounds.prompts.constantsManagerWarning.windowBox);
        anchor.prompts.constantsManagerInfoWindow = centeredPromptAnchorFrom(bounds.prompts.constantsManagerInfo.windowBox);

        anchor.prompts.overwriteWarningWindow = centeredPromptAnchorFrom(bounds.prompts.overwriteWarning.windowBox);
        anchor.prompts.infoWindow = centeredPromptAnchorFrom(bounds.prompts.infoWindow.windowBox);
        anchor.prompts.musicSettingWindow = centeredPromptAnchorFrom(bounds.prompts.musicSetting.windowBox);
    } /* prompts */

    /* ntoe canvas */ {

        bounds.noteCanvas.groupBox.width = window.interfaceRenderTextureWidth - anchor.noteCanvas.noteCanvas.x;
        bounds.noteCanvas.groupBox.height = anchor.navigationBar.status.y - anchor.noteCanvas.noteCanvas.y;

        systemState_.noteCanvas.gridCamera.offset = anchor.noteCanvas.noteCanvas;
        // systemState_.noteCanvas.gridCamera.target = {.0f, .0f};
        // systemState_.noteCanvas.gridCamera.rotation = .0f;
        // systemState_.noteCanvas.gridCamera.zoom = 1.0f;
        
		interfaceState_.noteCanvas.isGridLayoutDirty = true;

    } /* note canvas */

}

void MainWindow::handleKeyboardEvent(){

    if(IsKeyPressed(KEY_ESCAPE)){
        interfaceState_.prompts.isCommandWindowVisible = false;
        interfaceState_.prompts.isConstantsManagerWindowVisible = false;
        interfaceState_.prompts.isConstantsManagerWarningWindowVisible = false;
        interfaceState_.prompts.isConstantsManagerInfoWindowVisible = false;
        interfaceState_.prompts.isInfoWindowVisible = false;
        interfaceState_.prompts.isMusicSettingPromptVisible = false;
        if(interfaceState_.prompts.activeCommandPrompt == constants::prompts::CommandPrompt::Constant){
            interfaceState_.prompts.activeCommandPrompt = constants::prompts::CommandPrompt::Tempo;
            interfaceState_.prompts.selectedCommandTool = constants::prompts::CommandPrompt::Tempo;
        }
        if(canvasManager_) canvasManager_->cancelSelectionAndPasteMode();
        if(timelineManager_) timelineManager_->cancelActiveInteraction();

        interfaceState_.navigationBar.isPageSelectEnabled = false;
        interfaceState_.navigationBar.timelineSelectionStartIndex = constants::action_center::InvalidPageInsertionIndex;
        interfaceState_.navigationBar.timelineSelectionEndIndex = constants::action_center::InvalidPageInsertionIndex;
        
        return;
    }

    if(interfaceState_.prompts.isAnyPromptVisible()) return;

    const bool isControlDown{IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)};
    const bool isShiftDown{IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)};
    auto &window{systemState_.window};

    if(IsKeyPressed(KEY_LEFT) || (IsKeyPressed(KEY_A) && !isControlDown)){
        interfaceState_.navigationBar.requestedPageNumber = std::max(
            constants::interface_layout::timeline::FirstPageNumber,
            systemState_.project.currentPage - 1
        );
        machineState_.shouldResetPlayback = true;
    }
    if(IsKeyPressed(KEY_RIGHT) || (IsKeyPressed(KEY_S) && !IsKeyDown(KEY_LEFT_CONTROL))){
        const int maximumPageNumber{
            systemState_.project.data.lock()
                ? static_cast<int>(systemState_.project.data.lock()->data->pages.size())
                : constants::interface_layout::timeline::FirstPageNumber
        };
        interfaceState_.navigationBar.requestedPageNumber = std::min(
            maximumPageNumber,
            systemState_.project.currentPage + 1
        );
        machineState_.shouldResetPlayback = true;
    }

    if(IsKeyPressed(KEY_D)) interfaceState_.navigationBar.isPageRepeatEnabled = !interfaceState_.navigationBar.isPageRepeatEnabled;

    const float previousScaleFactor{window.scaleFactor};

    const bool isZoomInPressed{IsKeyPressed(KEY_KP_ADD) || IsKeyPressed(KEY_EQUAL)};
    const bool isZoomOutPressed{IsKeyPressed(KEY_KP_SUBTRACT) || IsKeyPressed(KEY_MINUS)};

    if(isControlDown && isShiftDown){
        if(isZoomInPressed){
            window.scaleFactor = std::min(std::max(
                constants::application_window::MinimumScaleFactor, std::floor(calculateMaximumScaleFactor())
            ), std::floor(window.scaleFactor) + 1.0f);
        }
        if(isZoomOutPressed) window.scaleFactor = std::max(constants::application_window::MinimumScaleFactor, std::ceil(window.scaleFactor) - 1.0f);

        if(!machineState_.isPlaying){
            if(IsKeyPressed(KEY_Z) && actionCenter_ && actionCenter_->redo()) applyProjectTransientNavigationState();
        }
    }else if(isControlDown){
        if(isZoomInPressed) window.scaleFactor += .1f; // TODO: magic numbers
        if(isZoomOutPressed) window.scaleFactor -= .1f;

        if(!machineState_.isPlaying){
            if(IsKeyPressed(KEY_Z) && actionCenter_ && actionCenter_->undo()) applyProjectTransientNavigationState();
        }
    }

    if(IsKeyPressed(KEY_SPACE)){
        machineState_.isPlaying = !machineState_.isPlaying;
        if(machineState_.isPlaying){
            if(canvasManager_) canvasManager_->cancelSelectionAndPasteMode();
            if(timelineManager_) timelineManager_->cancelActiveInteraction();
            interfaceState_.navigationBar.isTimelineDragCandidate = false;
            interfaceState_.navigationBar.isTimelineDraggingPage = false;
            interfaceState_.navigationBar.timelineDragCandidatePageIndex = constants::interface_layout::timeline::InvalidIndex;
            interfaceState_.navigationBar.timelineDraggedPageIndex = constants::interface_layout::timeline::InvalidIndex;
        }
    }

    // if(IsKeyPressed(KEY_UP)) window.scaleFactor += .1f;
    // if(IsKeyPressed(KEY_DOWN)) window.scaleFactor -= .1f;

    window.scaleFactor = calculateScaleFactor();
    if(window.scaleFactor != previousScaleFactor) handleWindowSizeChangeEvent();

    auto &navigationBarState{interfaceState_.navigationBar};
    const bool isPagesDomain{interfaceState_.activeSelectionDomain == program_states::Interface::SelectionDomain::Pages};
    
    const bool isPageCopy{navigationBarState.isPageCopyButtonPressed || (isPagesDomain && isControlDown && IsKeyPressed(KEY_C))};
    const bool isPageCut{navigationBarState.isPageCutButtonPressed || (isPagesDomain && isControlDown && IsKeyPressed(KEY_X))};
    const bool isPagePaste{navigationBarState.isPagePasteButtonPressed || (isPagesDomain && isControlDown && IsKeyPressed(KEY_V))};
    const bool isPageDelete{isPagesDomain && IsKeyPressed(KEY_DELETE)};

    navigationBarState.isPageCopyButtonPressed = false;
    navigationBarState.isPageCutButtonPressed = false;
    navigationBarState.isPagePasteButtonPressed = false;

    if((isPageCopy || isPageCut || isPageDelete) && actionCenter_){
        const int startIndex{navigationBarState.isPageSelectEnabled ? navigationBarState.timelineSelectionStartIndex : systemState_.project.currentPage - constants::action_center::FirstPageNumber};
        const int endIndex{navigationBarState.isPageSelectEnabled ? navigationBarState.timelineSelectionEndIndex : startIndex};

        if(startIndex != constants::action_center::InvalidPageInsertionIndex && endIndex != constants::action_center::InvalidPageInsertionIndex){
            if(isPageCopy) actionCenter_->copyPage(interfaceState_, startIndex, endIndex);
            else if(isPageCut) actionCenter_->cutPage(interfaceState_, startIndex, endIndex);
            else if(isPageDelete) actionCenter_->removePage(startIndex, endIndex);
        }

        if(navigationBarState.isPageSelectEnabled){
            if(isPageCopy && startIndex != constants::action_center::InvalidPageInsertionIndex && endIndex != constants::action_center::InvalidPageInsertionIndex){
                systemState_.project.currentPage = std::max(startIndex, endIndex) + constants::action_center::FirstPageNumber;
            }
            navigationBarState.isPageSelectEnabled = false;
            navigationBarState.timelineSelectionStartIndex = constants::action_center::InvalidPageInsertionIndex;
            navigationBarState.timelineSelectionEndIndex = constants::action_center::InvalidPageInsertionIndex;
        }
        
        if(isPageCopy){
            if(auto projectData{utilities::projectDataWithPagesFrom(systemState_)}){
                projectData->transient.currentPageNumber = systemState_.project.currentPage;
            }
        }
        
        applyProjectTransientNavigationState();
    }

    if(isPagePaste && !navigationBarState.isPageSelectEnabled && actionCenter_){
        actionCenter_->pastePage(interfaceState_, systemState_.project.currentPage - constants::action_center::FirstPageNumber + 1);
        applyProjectTransientNavigationState();
    }
}

void MainWindow::handleConstantsManagerEvents(){

    auto &state{interfaceState_.prompts};

    if(!state.hasRequestedCommit || !actionCenter_) return;

    state.hasRequestedCommit = false;

    DEBUG_PRINT("committing...");


    actionCenter_->commitConstantModification(state.draft);

    // state.draft.constantIndex = state.requestedConstantIndex;

}