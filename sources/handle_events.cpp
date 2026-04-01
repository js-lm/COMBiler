#include "main_window.hpp"

#include "constants.hpp"

void MainWindow::handleEvents(){
    if(IsWindowResized()) handleWindowSizeChangeEvent();


}

void MainWindow::handleWindowSizeChangeEvent(){
    using namespace constants::interface_layout;

    auto &window{systemState_.window};

    window.interfaceRenderTextureWidth  = GetScreenWidth();
    window.interfaceRenderTextureHeight = GetScreenHeight();

    UnloadRenderTexture(interfaceRenderTexture_);

    interfaceRenderTexture_ = LoadRenderTexture(
        window.interfaceRenderTextureWidth, 
        window.interfaceRenderTextureHeight
    );


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
    } /* toolbar */

    /* ntoe canvas */ {

        bounds.noteCanvas.groupBox.width = window.interfaceRenderTextureWidth - anchor.noteCanvas.noteCanvas.x;
        bounds.noteCanvas.groupBox.height = anchor.navigationBar.status.y - anchor.noteCanvas.noteCanvas.y;

    } /* note canvas */

}