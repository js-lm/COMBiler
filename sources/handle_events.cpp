#include "main_window.hpp"

#include "constants.hpp"

#include <algorithm>
#include <cmath>

void MainWindow::handleEvents(){
    
    handleKeyboardEvent();
    handleButtonPressEvents();

    if(IsWindowResized()) handleWindowSizeChangeEvent();


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

    const bool isControlDown{IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)};
    const bool isShiftDown{IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)};
    auto &window{systemState_.window};

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

        if(IsKeyPressed(KEY_Z) && actionCenter_ && actionCenter_->redo()) applyProjectTransientNavigationState();
    }else if(isControlDown){
        if(isZoomInPressed) window.scaleFactor += .1f;
        if(isZoomOutPressed) window.scaleFactor -= .1f;

        if(IsKeyPressed(KEY_Z) && actionCenter_ && actionCenter_->undo()) applyProjectTransientNavigationState();
    }

    // if(IsKeyPressed(KEY_UP)) window.scaleFactor += .1f;
    // if(IsKeyPressed(KEY_DOWN)) window.scaleFactor -= .1f;

    window.scaleFactor = calculateScaleFactor();

    if(window.scaleFactor != previousScaleFactor) handleWindowSizeChangeEvent();

}