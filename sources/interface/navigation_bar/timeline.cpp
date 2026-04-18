#include "navigation_bar.hpp"

#include <raygui.h>

#include "labels.hpp"

#include "interface/utilities.hpp"

#include "debug_utilities.hpp"

#include <algorithm>
#include <cmath>

#include "utilities/project_utilities.hpp"

#include "constants.hpp"

#include "timeline_manager/timeline_manager.hpp"

using namespace interface;
using namespace constants::labels::navigation_bar;

float NavigationBar::timelineBlockWidthInPixels(
    const program_states::ProjectData &projectData,
    size_t pageIndex
){
    const int pageNoteCount{
        utilities::currentPageNoteCountFrom(
            projectData,
            static_cast<int>(pageIndex) + constants::interface_layout::timeline::FirstPageNumber
        )
    };

    return std::max(
        constants::interface_layout::timeline::MinimumBlockWidthInPixels,
        static_cast<float>(pageNoteCount) * constants::interface_layout::timeline::BlockWidthPerNoteInPixels
    );

}

float NavigationBar::timelineContentWidthInPixels(const program_states::ProjectData &projectData){
    float contentWidthInPixels{constants::interface_layout::timeline::ContentLeftPaddingInPixels};

    for(size_t pageIndex{0}; pageIndex < projectData.pages.size(); pageIndex++){
        contentWidthInPixels += timelineBlockWidthInPixels(projectData, pageIndex);

        if(pageIndex + constants::interface_layout::timeline::FirstPageNumber < projectData.pages.size()){

            contentWidthInPixels += constants::interface_layout::timeline::BlockSpacingInPixels;
        }

    }

    contentWidthInPixels += constants::interface_layout::timeline::PlusButtonSizeInPixels;
    contentWidthInPixels += constants::interface_layout::timeline::PlusButtonOffsetInPixels;


    return contentWidthInPixels;
}

float NavigationBar::drawTimelineBlocksAndMarkers(
    program_states::InterfaceContext    &context,
    const program_states::ProjectData   &projectData,
    const Rectangle &scrollPanelBounds,
    const Vector2   &mousePosition,
    bool            isMouseInsideScrollPanel,
    int             &hoveredBlockIndex,
    int             &hoveredMarkerIndex,
    Rectangle       &draggedBlockBounds
){

    auto &navigationBarState{context.interface.navigationBar};

    float currentPositionX{constants::interface_layout::timeline::ContentLeftPaddingInPixels};

    for(size_t pageIndex{0}; pageIndex < projectData.pages.size(); pageIndex++){
        const float blockWidthInPixels{timelineBlockWidthInPixels(projectData, pageIndex)};
        const Rectangle blockBounds{
            scrollPanelBounds.x + navigationBarState.timelineScrollPanelScrollOffset.x + currentPositionX,
            scrollPanelBounds.y + navigationBarState.timelineScrollPanelScrollOffset.y + constants::interface_layout::timeline::ContentTopPaddingInPixels,
            blockWidthInPixels,
            constants::interface_layout::timeline::BlockHeightInPixels
        };

        const bool isBlockHovered{isMouseInsideScrollPanel && CheckCollisionPointRec(mousePosition, blockBounds)};
        if(isBlockHovered){

            hoveredBlockIndex = static_cast<int>(pageIndex);
        }

        if(!navigationBarState.isTimelineDraggingPage || navigationBarState.timelineDraggedPageIndex != static_cast<int>(pageIndex)){
            GuiButton(blockBounds, "");

            DrawRectangleLinesEx(
                blockBounds,
                constants::interface_layout::timeline::BlockBorderThicknessInPixels,
                (static_cast<int>(pageIndex) + constants::interface_layout::timeline::FirstPageNumber == context.system.project.currentPage)
                    ? constants::interface_layout::note_canvas::selection::BorderColor
                    : GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL))
            );
        }else{

            draggedBlockBounds = Rectangle{
                mousePosition.x - navigationBarState.timelineDragGrabOffsetInPixelsX,
                blockBounds.y,
                blockBounds.width,
                blockBounds.height
            };
        }

        if(pageIndex + constants::interface_layout::timeline::FirstPageNumber < projectData.pages.size()){

            const Rectangle markerBounds{
                blockBounds.x + blockBounds.width
              + (constants::interface_layout::timeline::BlockSpacingInPixels * constants::interface_layout::timeline::MarkerHalfWidthScale)
              - (constants::interface_layout::timeline::InsertionMarkerWidthInPixels * constants::interface_layout::timeline::MarkerHalfWidthScale),
                scrollPanelBounds.y + navigationBarState.timelineScrollPanelScrollOffset.y
              + constants::interface_layout::timeline::ContentTopPaddingInPixels
              + constants::interface_layout::timeline::InsertionMarkerTopOffsetInPixels,
                constants::interface_layout::timeline::InsertionMarkerWidthInPixels,
                constants::interface_layout::timeline::InsertionMarkerHeightInPixels
            };

            const bool isMarkerHovered{isMouseInsideScrollPanel && CheckCollisionPointRec(mousePosition, markerBounds)};
            if(isMarkerHovered){
                hoveredMarkerIndex = static_cast<int>(pageIndex) + constants::interface_layout::timeline::FirstPageNumber;
            }


            DrawRectangleRec(
                markerBounds,
                isMarkerHovered
                    ? constants::interface_layout::note_canvas::selection::BorderColor
                    : GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL))
            );

            currentPositionX += blockWidthInPixels + constants::interface_layout::timeline::BlockSpacingInPixels;

        }else{

            currentPositionX += blockWidthInPixels;

        }
    }

    return currentPositionX;
}

void NavigationBar::handleTimelineMousePress(
    program_states::InterfaceContext   &context,
    program_states::ProjectData        &projectData,
    const Rectangle &scrollPanelBounds,
    const Vector2   &mousePosition,
    bool            isMouseInsideScrollPanel,
    int             hoveredBlockIndex,
    int             hoveredMarkerIndex
){
    if(!isMouseInsideScrollPanel || !IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){

        return;
    }

    auto &navigationBarState{context.interface.navigationBar};

    if(hoveredBlockIndex >= constants::interface_layout::timeline::MinimumIndex && !navigationBarState.isPageSelectEnabled){
        navigationBarState.isTimelineDragCandidate = true;
        navigationBarState.timelineDragCandidatePageIndex = hoveredBlockIndex;
        navigationBarState.timelineDragStartMouseScreenPosition = mousePosition;

        float grabPositionX{constants::interface_layout::timeline::ContentLeftPaddingInPixels};
        for(size_t pageIndex{0}; pageIndex <= static_cast<size_t>(hoveredBlockIndex); pageIndex++){
            if(pageIndex == static_cast<size_t>(hoveredBlockIndex)){
                navigationBarState.timelineDragGrabOffsetInPixelsX = mousePosition.x
                                                                   - (
                                                                        scrollPanelBounds.x 
                                                                      + navigationBarState.timelineScrollPanelScrollOffset.x 
                                                                      + grabPositionX
                                                                    );

                break;
            }

            grabPositionX += timelineBlockWidthInPixels(projectData, pageIndex)
                           + constants::interface_layout::timeline::BlockSpacingInPixels;

        }
    }else if(hoveredBlockIndex >= constants::interface_layout::timeline::MinimumIndex){
        navigationBarState.requestedPageNumber = hoveredBlockIndex + constants::interface_layout::timeline::FirstPageNumber;
    }

    if(hoveredMarkerIndex >= constants::interface_layout::timeline::MinimumIndex){
        const double currentTimeInSeconds{GetTime()};
        const bool isDoubleClick{
            navigationBarState.timelinePreviousClickedInsertionIndex == hoveredMarkerIndex
                && (currentTimeInSeconds - navigationBarState.timelinePreviousInsertionClickTimeInSeconds)
                   <= constants::interface_layout::timeline::InsertionDoubleClickThresholdInSeconds

        };


        if(isDoubleClick){
            projectData.pages.insert(projectData.pages.begin() + hoveredMarkerIndex, program_states::ProjectData::Page{});
            projectData.metadata.numberOfPages = static_cast<int>(projectData.pages.size());
            navigationBarState.requestedPageNumber = hoveredMarkerIndex + constants::interface_layout::timeline::FirstPageNumber;
            context.interface.noteCanvas.isGridLayoutDirty = true;
        }

        navigationBarState.timelinePreviousClickedInsertionIndex = hoveredMarkerIndex;
        navigationBarState.timelinePreviousInsertionClickTimeInSeconds = currentTimeInSeconds;

    }



}

void NavigationBar::updateTimelineDragCandidateState(
    program_states::InterfaceContext &context,
    int hoveredMarkerIndex
){

    auto &navigationBarState{context.interface.navigationBar};

    if(!navigationBarState.isTimelineDragCandidate) return;

    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){

        const Vector2 mousePosition{GetMousePosition()};

        const float deltaX{mousePosition.x - navigationBarState.timelineDragStartMouseScreenPosition.x};
        const float deltaY{mousePosition.y - navigationBarState.timelineDragStartMouseScreenPosition.y};
        const float dragDistanceInPixels{std::sqrt((deltaX * deltaX) + (deltaY * deltaY))};

        if(dragDistanceInPixels >= constants::interface_layout::timeline::DragStartDistanceThresholdInPixels){
            navigationBarState.isTimelineDraggingPage = true;
            navigationBarState.timelineDraggedPageIndex = navigationBarState.timelineDragCandidatePageIndex;
            navigationBarState.timelineDropInsertionIndex = hoveredMarkerIndex >= constants::interface_layout::timeline::MinimumIndex
                                                                ? hoveredMarkerIndex
                                                                : navigationBarState.timelineDraggedPageIndex;
            navigationBarState.isTimelineDragCandidate = false;

        }
    }

    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
        navigationBarState.requestedPageNumber = navigationBarState.timelineDragCandidatePageIndex
                                               + constants::interface_layout::timeline::FirstPageNumber;
        context.interface.noteCanvas.isGridLayoutDirty = true;
        navigationBarState.isTimelineDragCandidate = false;
    }

}

void NavigationBar::updateTimelineDraggingState(
    program_states::InterfaceContext &context,
    program_states::ProjectData     &projectData,
    int                             hoveredMarkerIndex,
    const Rectangle                 &draggedBlockBounds
){

    auto &navigationBarState{context.interface.navigationBar};

    if(!navigationBarState.isTimelineDraggingPage) return;
    

    if(hoveredMarkerIndex >= constants::interface_layout::timeline::MinimumIndex){
        navigationBarState.timelineDropInsertionIndex = hoveredMarkerIndex;
    }

    const bool canDropPage{
        IsMouseButtonReleased(MOUSE_BUTTON_LEFT)
     && navigationBarState.timelineDraggedPageIndex >= constants::interface_layout::timeline::MinimumIndex
     && navigationBarState.timelineDraggedPageIndex < static_cast<int>(projectData.pages.size())
    };

    if(canDropPage){
        auto movedPage{projectData.pages[navigationBarState.timelineDraggedPageIndex]};
        projectData.pages.erase(projectData.pages.begin() + navigationBarState.timelineDraggedPageIndex);

        const int insertionIndexOffset{
            navigationBarState.timelineDraggedPageIndex < navigationBarState.timelineDropInsertionIndex
                ? constants::interface_layout::timeline::FirstPageNumber
                : constants::interface_layout::timeline::MinimumIndex
        };
        const int insertionIndex{std::clamp(
            navigationBarState.timelineDropInsertionIndex - insertionIndexOffset,
            constants::interface_layout::timeline::MinimumIndex,
            static_cast<int>(projectData.pages.size())
        )};

        projectData.pages.insert(projectData.pages.begin() + insertionIndex, std::move(movedPage));
        navigationBarState.requestedPageNumber = insertionIndex + constants::interface_layout::timeline::FirstPageNumber;
        context.interface.noteCanvas.isGridLayoutDirty = true;
        navigationBarState.isTimelineDraggingPage = false;

    }else{

        
        DrawRectangleRec(draggedBlockBounds, GetColor(GuiGetStyle(BUTTON, BASE_COLOR_PRESSED)));
        DrawRectangleLinesEx(
            draggedBlockBounds,
            constants::interface_layout::timeline::BlockBorderThicknessInPixels,
            GetColor(GuiGetStyle(BUTTON, BORDER_COLOR_FOCUSED))
        );

    }

}

void NavigationBar::drawTimeline(program_states::InterfaceContext &context){
    auto &navigationBarState{context.interface.navigationBar};
    const auto timelineAnchor{context.layout.anchor.navigationBar.timeline};
    const auto &timelineBounds{context.layout.bounds.navigationBar.timeline};
    const auto scrollPanelBounds{calculateBoundsAtAnchor(timelineAnchor, timelineBounds.scrollPanel)};

    const auto projectData{utilities::projectDataWithPagesFrom(context.system)};
    const float contentWidthInPixels{
        projectData
            ? timelineContentWidthInPixels(*projectData)
            : constants::interface_layout::timeline::ContentLeftPaddingInPixels
    };

    if(NavigationBar::timelineManager_){
        navigationBarState.timelineScrollPanelScrollOffset = NavigationBar::timelineManager_->viewState().scrollOffsetInPixels;
    }

    GuiGroupBox(calculateBoundsAtAnchor(timelineAnchor, timelineBounds.groupBox), TimelineGroupBoxText);
    GuiStatusBar(calculateBoundsAtAnchor(timelineAnchor, timelineBounds.statusBar), TimelineStatusBarText);
    GuiScrollPanel(
        Rectangle{
            scrollPanelBounds.x,
            scrollPanelBounds.y,
            scrollPanelBounds.width - navigationBarState.timelineScrollPanelBoundsOffset.x,
            scrollPanelBounds.height - navigationBarState.timelineScrollPanelBoundsOffset.y
        },
        nullptr,
        Rectangle{
            constants::interface_layout::timeline::ContentOriginXInPixels,
            constants::interface_layout::timeline::ContentOriginYInPixels,
            std::max(scrollPanelBounds.width, contentWidthInPixels),
            scrollPanelBounds.height - constants::interface_layout::timeline::StaticCornerSizeInPixels
        },
        &navigationBarState.timelineScrollPanelScrollOffset,
        &navigationBarState.timelineScrollPanelScrollView
    );


    const Rectangle staticCornerBounds{
        scrollPanelBounds.x + scrollPanelBounds.width - constants::interface_layout::timeline::StaticCornerSizeInPixels,
        scrollPanelBounds.y + scrollPanelBounds.height - constants::interface_layout::timeline::StaticCornerSizeInPixels,
        constants::interface_layout::timeline::StaticCornerSizeInPixels,
        constants::interface_layout::timeline::StaticCornerSizeInPixels
    };
    DrawRectangleRec(staticCornerBounds, GetColor(GuiGetStyle(STATUSBAR, BASE_COLOR_NORMAL)));
    DrawRectangleLinesEx(
        staticCornerBounds,
        constants::interface_layout::timeline::StaticCornerBorderThicknessInPixels,
        GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL))
    );

    const Vector2 mousePosition{GetMousePosition()};
    const bool isMouseInsideScrollPanel{CheckCollisionPointRec(mousePosition, scrollPanelBounds)};
    int hoveredMarkerIndex{constants::interface_layout::timeline::InvalidIndex};
    int hoveredBlockIndex{constants::interface_layout::timeline::InvalidIndex};
    Rectangle draggedBlockBounds{};
    float contentRightEdgePositionInPixels{constants::interface_layout::timeline::ContentLeftPaddingInPixels};


    BeginScissorMode(
        static_cast<int>(scrollPanelBounds.x),
        static_cast<int>(scrollPanelBounds.y),
        static_cast<int>(scrollPanelBounds.width),
        static_cast<int>(scrollPanelBounds.height)
    ); {

        if(projectData){
            contentRightEdgePositionInPixels = drawTimelineBlocksAndMarkers(
                context,
                *projectData,
                scrollPanelBounds,
                mousePosition,
                isMouseInsideScrollPanel,
                hoveredBlockIndex,
                hoveredMarkerIndex,
                draggedBlockBounds
            );

            if(GuiButton(
                Rectangle{
                    scrollPanelBounds.x + navigationBarState.timelineScrollPanelScrollOffset.x + contentRightEdgePositionInPixels
                  + constants::interface_layout::timeline::PlusButtonOffsetInPixels,
                    scrollPanelBounds.y + navigationBarState.timelineScrollPanelScrollOffset.y
                  + constants::interface_layout::timeline::ContentTopPaddingInPixels
                  + ((constants::interface_layout::timeline::BlockHeightInPixels
                    - constants::interface_layout::timeline::PlusButtonSizeInPixels)
                    * constants::interface_layout::timeline::PlusButtonVerticalCenterScale),
                    constants::interface_layout::timeline::PlusButtonSizeInPixels,
                    constants::interface_layout::timeline::PlusButtonSizeInPixels
                },
                "+"
            )){
                projectData->pages.push_back(program_states::ProjectData::Page{});
                projectData->metadata.numberOfPages = static_cast<int>(projectData->pages.size());
                navigationBarState.requestedPageNumber = static_cast<int>(projectData->pages.size());

                context.interface.noteCanvas.isGridLayoutDirty = true;

            }

        }
    } EndScissorMode();

    if(projectData){

        handleTimelineMousePress(
            context,
            *projectData,
            scrollPanelBounds,
            mousePosition,
            isMouseInsideScrollPanel,
            hoveredBlockIndex,
            hoveredMarkerIndex
        );
        updateTimelineDragCandidateState(context, hoveredMarkerIndex);
        updateTimelineDraggingState(context, *projectData, hoveredMarkerIndex, draggedBlockBounds);

    }


    GuiLabel(calculateBoundsAtAnchor(timelineAnchor, timelineBounds.timelineLabel), TimelineLabelText);
    GuiLabel(calculateBoundsAtAnchor(timelineAnchor, timelineBounds.pageNumberLabel), PageNumberLabelText);
    const int maximumPageNumber{
        projectData
            ? static_cast<int>(projectData->pages.size())
            : constants::interface_layout::timeline::FirstPageNumber
    };
    GuiLabel(
        calculateBoundsAtAnchor(timelineAnchor, timelineBounds.pageNumberValueLabel),
        TextFormat("%d/%d", context.system.project.currentPage, maximumPageNumber)
    );

    navigationBarState.isPageCopyButtonPressed = GuiButton(calculateBoundsAtAnchor(timelineAnchor, timelineBounds.pageCopyButton), PageCopyButtonText);
    navigationBarState.isPagePasteButtonPressed = GuiButton(calculateBoundsAtAnchor(timelineAnchor, timelineBounds.pagePasteButton), PagePasteButtonText);
    navigationBarState.isPageCutButtonPressed = GuiButton(calculateBoundsAtAnchor(timelineAnchor, timelineBounds.pageCutButton), PageCutButtonText);
    GuiToggle(calculateBoundsAtAnchor(timelineAnchor, timelineBounds.pageSelectToggle), PageSelectToggleText, &navigationBarState.isPageSelectEnabled);

    DEBUG_PRINT_IF_CHANGED(
        "NavigationBar::drawTimeline()\n\tmaximumPageNumber={}, contentWidthInPixels={}, hoveredBlockIndex={}, hoveredMarkerIndex={},\n\tisTimelineDragCandidate={}, isTimelineDraggingPage={}, timelineDraggedPageIndex={}, timelineDropInsertionIndex={}, requestedPageNumber={}",
        maximumPageNumber,
        contentWidthInPixels,
        hoveredBlockIndex,
        hoveredMarkerIndex,
        navigationBarState.isTimelineDragCandidate,
        navigationBarState.isTimelineDraggingPage,
        navigationBarState.timelineDraggedPageIndex,
        navigationBarState.timelineDropInsertionIndex,
        navigationBarState.requestedPageNumber
    );

}
