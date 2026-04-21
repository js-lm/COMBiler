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

float NavigationBar::timelineBlockPositionXInPixels(
    const program_states::ProjectData &projectData,
    size_t targetPageIndex
){
    float positionXInPixels{constants::interface_layout::timeline::ContentLeftPaddingInPixels};

    for(size_t pageIndex{constants::interface_layout::timeline::MinimumIndex};
        pageIndex < targetPageIndex;
        pageIndex++
    ){
        positionXInPixels += timelineBlockWidthInPixels(projectData, pageIndex);
        positionXInPixels += constants::interface_layout::timeline::BlockSpacingInPixels;
    }

    return positionXInPixels;
}

float NavigationBar::timelineContentWidthInPixels(const program_states::ProjectData &projectData){
    if(projectData.pages.empty()){
        return constants::interface_layout::timeline::ContentLeftPaddingInPixels;
    }

    return timelineBlockPositionXInPixels(projectData, projectData.pages.size())
         - constants::interface_layout::timeline::BlockSpacingInPixels;  
}

int NavigationBar::wrappedPreviewRowFromNote(
    music_data::Note note,
    int previewRowCount
){
    if(previewRowCount <= 0){
        return constants::interface_layout::timeline::MinimumIndex;
    }

    const int semitoneIndex{static_cast<int>(note)};
    const int wrappedRowIndex{semitoneIndex % previewRowCount};

    return wrappedRowIndex;
}

Color NavigationBar::commandPreviewColor(const command::CommandToken &commandToken){

    return std::visit([](const auto &commandToken)->Color{
        using Type = std::decay_t<decltype(commandToken)>;

        if constexpr(std::is_same_v<Type, command::Command>) return BLANK;
        
        constexpr auto previewOverlayAlpha{constants::interface_layout::timeline::PreviewOverlayAlpha};
        
        if constexpr(std::is_same_v<Type, command::Tempo>){
            return Fade(constants::interface_layout::note_canvas::commands::TempoColor, previewOverlayAlpha);
        }else if constexpr(std::is_same_v<Type, command::Volume>){
            return Fade(constants::interface_layout::note_canvas::commands::VolumeColor, previewOverlayAlpha);
        }

        return Fade(constants::interface_layout::note_canvas::commands::ArticulationColor, previewOverlayAlpha);
        
    }, commandToken);
}

void NavigationBar::drawTimelineBlockPreview(
    const program_states::ProjectData::Page &page,
    const Rectangle &blockBounds,
    int noteCountInThisPage
){
    if(noteCountInThisPage <= 0) return;

    const float columnWidthInPixels{blockBounds.width / static_cast<float>(noteCountInThisPage)};
    if(columnWidthInPixels <= .0f) return;

    constexpr float noteHeightInPixels{constants::interface_layout::timeline::PreviewNoteHeightInPixels};
    const int notePreviewRowCount{
        std::max(
            constants::interface_layout::timeline::FirstPageNumber,
            static_cast<int>(blockBounds.height / noteHeightInPixels)
        )
    };

    for(int noteIndex{0}; noteIndex < noteCountInThisPage; noteIndex++){
        const float columnPositionX{blockBounds.x + (columnWidthInPixels * static_cast<float>(noteIndex))};

        if(noteIndex < page.commandChannel.size() && page.commandChannel[noteIndex].has_value()){
            
            DrawRectangleRec(
                Rectangle{
                    columnPositionX,
                    blockBounds.y,
                    std::max(constants::interface_layout::timeline::PreviewMinimumColumnWidthInPixels, columnWidthInPixels),
                    blockBounds.height
                },
                commandPreviewColor(page.commandChannel[noteIndex].value())
            );
        }

        for(int channelIndex{constants::project_data::NumberOfInstrumentChannels - 1};
            channelIndex >= constants::interface_layout::timeline::MinimumIndex;
            channelIndex--
        ){

            const auto &instrumentChannel{page.instrumentChannels[static_cast<size_t>(channelIndex)]};
            if(noteIndex >= instrumentChannel.size() || !instrumentChannel[noteIndex].has_value()){
                continue;
            }

            const auto &channelData{instrumentChannel[noteIndex].value()};
            const Color channelColor{constants::interface_layout::note_canvas::notes::ChannelNoteColors[static_cast<size_t>(channelIndex)]};
            constexpr float previewOverlayAlpha{constants::interface_layout::timeline::PreviewOverlayAlpha};

            if(std::holds_alternative<music_data::Instrument>(channelData)){
                
                DrawRectangleRec(
                    Rectangle{
                        columnPositionX,
                        blockBounds.y,
                        std::max(constants::interface_layout::timeline::PreviewMinimumColumnWidthInPixels, columnWidthInPixels),
                        blockBounds.height
                    },
                    Fade(channelColor, previewOverlayAlpha)
                );
            }

        }

        for(int channelIndex{constants::project_data::NumberOfInstrumentChannels - 1};
            channelIndex >= constants::interface_layout::timeline::MinimumIndex;
            channelIndex--
        ){

            const auto &instrumentChannel{page.instrumentChannels[channelIndex]};
            if(noteIndex >= instrumentChannel.size() || !instrumentChannel[noteIndex].has_value()){
                continue;
            }

            const auto &channelData{instrumentChannel[noteIndex].value()};
            if(!std::holds_alternative<music_data::Note>(channelData)){
                continue;
            }

            const auto note{std::get<music_data::Note>(channelData)};
            const int wrappedRowIndex{wrappedPreviewRowFromNote(note, notePreviewRowCount)};
            const float notePositionY{
                blockBounds.y + blockBounds.height
              - (static_cast<float>(wrappedRowIndex + constants::interface_layout::timeline::FirstPageNumber) * noteHeightInPixels)
            };

            DrawRectangleRec(
                Rectangle{
                    columnPositionX,
                    notePositionY,
                    std::max(constants::interface_layout::timeline::PreviewMinimumColumnWidthInPixels, columnWidthInPixels),
                    noteHeightInPixels
                },
                constants::interface_layout::note_canvas::notes::ChannelNoteColors[static_cast<size_t>(channelIndex)]
            );
        }

    }


}

float NavigationBar::timelineDropIndicatorCenterPositionXInPixels(
    const program_states::ProjectData &projectData,
    const Rectangle &scrollPanelBounds,
    float   scrollOffsetXInPixels,
    int     dropInsertionIndex
){
    const int clampedDropInsertionIndex{
        std::clamp(
            dropInsertionIndex,
            constants::interface_layout::timeline::MinimumIndex,
            static_cast<int>(projectData.pages.size())
        )
    };

    const float positionXInPixels{
        scrollPanelBounds.x + scrollOffsetXInPixels + timelineBlockPositionXInPixels(projectData, clampedDropInsertionIndex)
    };

    const float halfSpacingInPixels{
        constants::interface_layout::timeline::BlockSpacingInPixels * constants::interface_layout::timeline::MarkerHalfWidthScale
    };

    return positionXInPixels - halfSpacingInPixels;

}

int NavigationBar::timelineDropInsertionIndexFromMouseXInPixels(
    const program_states::ProjectData &projectData,
    const Rectangle &scrollPanelBounds,
    float scrollOffsetXInPixels,
    float mousePositionXInPixels
){
    if(projectData.pages.empty()){
        return constants::interface_layout::timeline::MinimumIndex;
    }

    float currentPositionX{constants::interface_layout::timeline::ContentLeftPaddingInPixels};
    for(size_t pageIndex{constants::interface_layout::timeline::MinimumIndex};
        pageIndex < projectData.pages.size();
        pageIndex++
    ){
        const float blockWidthInPixels{timelineBlockWidthInPixels(projectData, pageIndex)};
        const float blockCenterPositionXInPixels{
            scrollPanelBounds.x + scrollOffsetXInPixels + currentPositionX
          + (blockWidthInPixels * constants::interface_layout::timeline::MarkerHalfWidthScale)
        };

        if(mousePositionXInPixels < blockCenterPositionXInPixels){ 
            return pageIndex;
        }

        currentPositionX += blockWidthInPixels + constants::interface_layout::timeline::BlockSpacingInPixels;
    }

    return projectData.pages.size();
}

float NavigationBar::drawTimelineBlocksAndMarkers(
    program_states::InterfaceContext    &context,
    const program_states::ProjectData   &projectData,
    const Rectangle &scrollPanelBounds,
    const Vector2   &mousePosition,
    bool            isMouseInsideScrollPanel,
    int             &hoveredBlockIndex,
    Rectangle       &draggedBlockBounds
){
    auto &navigationBarState{context.interface.navigationBar};
    float currentPositionX{constants::interface_layout::timeline::ContentLeftPaddingInPixels};

    for(size_t pageIndex{constants::interface_layout::timeline::MinimumIndex};
        pageIndex < projectData.pages.size();
        pageIndex++
    ){
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
            DrawRectangleRec(
                blockBounds,
                GetColor(GuiGetStyle(BUTTON, BASE_COLOR_NORMAL))
            );

            const int noteCountInThisPage{
                std::clamp(
                    projectData.pages[pageIndex].noteInThisPage.value_or(projectData.metadata.notePerPage),
                    constants::project_data::MinimumNotePerPage, 
                    constants::project_data::MaximumNotePerPage
                )
            };
            drawTimelineBlockPreview(projectData.pages[pageIndex], blockBounds, noteCountInThisPage);
            DrawRectangleLinesEx(
                blockBounds,
                constants::interface_layout::timeline::BlockBorderThicknessInPixels,
                (static_cast<int>(pageIndex) + constants::interface_layout::timeline::FirstPageNumber == context.system.project.currentPage)
                    ? constants::interface_layout::note_canvas::selection::BorderColor
                    : GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL))
            );
        }else{
            draggedBlockBounds = Rectangle{
                mousePosition.x + constants::interface_layout::timeline::DraggedBlockCursorOffsetXInPixels,
                mousePosition.y + constants::interface_layout::timeline::DraggedBlockCursorOffsetYInPixels,
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

            DrawRectangleRec(
                markerBounds,
                GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL))
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
    const program_states::ProjectData  &projectData,
    const Rectangle &scrollPanelBounds,
    const Vector2   &mousePosition,
    bool            isMouseInsideScrollPanel,
    int             hoveredBlockIndex
){
    if(!isMouseInsideScrollPanel || !IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        return;
    }

    auto &navigationBarState{context.interface.navigationBar};

    if(hoveredBlockIndex < constants::interface_layout::timeline::MinimumIndex){
        return;
    }

    if(navigationBarState.isPageSelectEnabled){
        navigationBarState.requestedPageNumber = hoveredBlockIndex + constants::interface_layout::timeline::FirstPageNumber;
        return;
    }

    if(hoveredBlockIndex >= constants::interface_layout::timeline::MinimumIndex
    && !navigationBarState.isPageSelectEnabled
    ){
        navigationBarState.isTimelineDragCandidate = true;
        navigationBarState.timelineDragCandidatePageIndex = hoveredBlockIndex;
        navigationBarState.timelineDragStartMouseScreenPosition = mousePosition;
        navigationBarState.timelineDragCandidateStartTimeInSeconds = GetTime();

        navigationBarState.timelineDragGrabOffsetInPixelsX = mousePosition.x
                                                           - (scrollPanelBounds.x 
                                                            + navigationBarState.timelineScrollPanelScrollOffset.x 
                                                            + timelineBlockPositionXInPixels(projectData, hoveredBlockIndex)
                                                           );
    }
}

void NavigationBar::updateTimelineDragCandidateState(
    program_states::InterfaceContext &context
){
    auto &navigationBarState{context.interface.navigationBar};

    if(IsKeyPressed(KEY_ESCAPE)){
        navigationBarState.isTimelineDragCandidate = false;
        navigationBarState.isTimelineDraggingPage = false;
        navigationBarState.timelineDragCandidatePageIndex = constants::interface_layout::timeline::InvalidIndex;
        navigationBarState.timelineDraggedPageIndex = constants::interface_layout::timeline::InvalidIndex;
        return;
    }

    if(!navigationBarState.isTimelineDragCandidate){
        return;
    }

    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
        navigationBarState.requestedPageNumber = navigationBarState.timelineDragCandidatePageIndex
                                               + constants::interface_layout::timeline::FirstPageNumber;
        context.interface.noteCanvas.isGridLayoutDirty = true;
        navigationBarState.isTimelineDragCandidate = false;
        navigationBarState.timelineDragCandidatePageIndex = constants::interface_layout::timeline::InvalidIndex;
        return;
    }

    if(!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) return;

    const Vector2 mousePosition{GetMousePosition()};
    const double currentTimeInSeconds{GetTime()};

    const float deltaX{mousePosition.x - navigationBarState.timelineDragStartMouseScreenPosition.x};
    const float deltaY{mousePosition.y - navigationBarState.timelineDragStartMouseScreenPosition.y};
    const float dragDistanceInPixels{std::sqrt((deltaX * deltaX) + (deltaY * deltaY))};
    const bool hasHoldDurationReached{
        (currentTimeInSeconds - navigationBarState.timelineDragCandidateStartTimeInSeconds)
            >= constants::interface_layout::timeline::HoldToStartDragDurationInSeconds
    };

    if(dragDistanceInPixels >= constants::interface_layout::timeline::DragStartDistanceThresholdInPixels || hasHoldDurationReached){
        navigationBarState.isTimelineDraggingPage = true;
        navigationBarState.timelineDraggedPageIndex = navigationBarState.timelineDragCandidatePageIndex;
        navigationBarState.timelineDropInsertionIndex = navigationBarState.timelineDraggedPageIndex;
        navigationBarState.isTimelineDragCandidate = false;
    }
}

void NavigationBar::updateTimelineDraggingState(
    program_states::InterfaceContext &context,
    program_states::ProjectData     &projectData,
    const Rectangle                 &scrollPanelBounds,
    const Vector2                   &mousePosition,
    const Rectangle                 &draggedBlockBounds
){
    auto &navigationBarState{context.interface.navigationBar};

    if(!navigationBarState.isTimelineDraggingPage) return;

    if(IsKeyPressed(KEY_ESCAPE)){
        navigationBarState.isTimelineDraggingPage = false;
        navigationBarState.timelineDraggedPageIndex = constants::interface_layout::timeline::InvalidIndex;
        return;
    }

    navigationBarState.timelineDropInsertionIndex = timelineDropInsertionIndexFromMouseXInPixels(
        projectData,
        scrollPanelBounds,
        navigationBarState.timelineScrollPanelScrollOffset.x,
        mousePosition.x
    );

    if(projectData.pages.size() >= static_cast<size_t>(constants::interface_layout::timeline::FirstPageNumber)){
        const int maximumInsertionIndex{static_cast<int>(projectData.pages.size())};
        navigationBarState.timelineDropInsertionIndex = std::clamp(
            navigationBarState.timelineDropInsertionIndex,
            constants::interface_layout::timeline::MinimumIndex,
            maximumInsertionIndex
        );

        const float dropIndicatorCenterXInPixels{timelineDropIndicatorCenterPositionXInPixels(
            projectData,
            scrollPanelBounds,
            navigationBarState.timelineScrollPanelScrollOffset.x,
            navigationBarState.timelineDropInsertionIndex
        )};
        const float dropIndicatorTopYInPixels{
            scrollPanelBounds.y + navigationBarState.timelineScrollPanelScrollOffset.y
          + constants::interface_layout::timeline::ContentTopPaddingInPixels
          - constants::interface_layout::timeline::DropIndicatorVerticalExtraHeightInPixels
        };
        const float dropIndicatorBottomYInPixels{
            scrollPanelBounds.y + navigationBarState.timelineScrollPanelScrollOffset.y
          + constants::interface_layout::timeline::ContentTopPaddingInPixels
          + constants::interface_layout::timeline::BlockHeightInPixels
          + constants::interface_layout::timeline::DropIndicatorVerticalExtraHeightInPixels
        };
        const float dropIndicatorCapLeftXInPixels{
            dropIndicatorCenterXInPixels
          - (constants::interface_layout::timeline::DropIndicatorCapWidthInPixels * constants::interface_layout::timeline::MarkerHalfWidthScale)
        };
        const float dropIndicatorCapRightXInPixels{
            dropIndicatorCenterXInPixels
          + (constants::interface_layout::timeline::DropIndicatorCapWidthInPixels * constants::interface_layout::timeline::MarkerHalfWidthScale)
        };

        DrawLineEx(
            Vector2{dropIndicatorCenterXInPixels, dropIndicatorTopYInPixels},
            Vector2{dropIndicatorCenterXInPixels, dropIndicatorBottomYInPixels},
            constants::interface_layout::timeline::DropIndicatorLineThicknessInPixels,
            RED
        );
        DrawLineEx(
            Vector2{dropIndicatorCapLeftXInPixels, dropIndicatorTopYInPixels},
            Vector2{dropIndicatorCapRightXInPixels, dropIndicatorTopYInPixels},
            constants::interface_layout::timeline::DropIndicatorLineThicknessInPixels,
            RED
        );
        DrawLineEx(
            Vector2{dropIndicatorCapLeftXInPixels, dropIndicatorBottomYInPixels},
            Vector2{dropIndicatorCapRightXInPixels, dropIndicatorBottomYInPixels},
            constants::interface_layout::timeline::DropIndicatorLineThicknessInPixels,
            RED
        );
    }

    const bool canDropPage{
        IsMouseButtonReleased(MOUSE_BUTTON_LEFT)
     && navigationBarState.timelineDraggedPageIndex >= constants::interface_layout::timeline::MinimumIndex
     && navigationBarState.timelineDraggedPageIndex < static_cast<int>(projectData.pages.size())
    };

    if(canDropPage){
        navigationBarState.isPageMoveRequested = true;
        navigationBarState.requestedPageMoveSourceIndex = navigationBarState.timelineDraggedPageIndex;
        navigationBarState.requestedPageMoveInsertionIndex = navigationBarState.timelineDropInsertionIndex;
        context.interface.noteCanvas.isGridLayoutDirty = true;
        navigationBarState.isTimelineDraggingPage = false;
        navigationBarState.timelineDraggedPageIndex = constants::interface_layout::timeline::InvalidIndex;
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
    int hoveredBlockIndex{constants::interface_layout::timeline::InvalidIndex};
    Rectangle draggedBlockBounds{};


    BeginScissorMode(
        static_cast<int>(scrollPanelBounds.x),
        static_cast<int>(scrollPanelBounds.y),
        static_cast<int>(scrollPanelBounds.width),
        static_cast<int>(scrollPanelBounds.height)
    ); {

        if(projectData){
            drawTimelineBlocksAndMarkers(
                context,
                *projectData,
                scrollPanelBounds,
                mousePosition,
                isMouseInsideScrollPanel,
                hoveredBlockIndex,
                draggedBlockBounds
            );

        }
    } EndScissorMode();

    if(projectData){
        if(navigationBarState.isPageSelectEnabled){
            navigationBarState.isTimelineDragCandidate = false;
            navigationBarState.isTimelineDraggingPage = false;
            navigationBarState.timelineDragCandidatePageIndex = constants::interface_layout::timeline::InvalidIndex;
            navigationBarState.timelineDraggedPageIndex = constants::interface_layout::timeline::InvalidIndex;
        }

        handleTimelineMousePress(
            context,
            *projectData,
            scrollPanelBounds,
            mousePosition,
            isMouseInsideScrollPanel,
            hoveredBlockIndex
        );
        if(!navigationBarState.isPageSelectEnabled){
            updateTimelineDragCandidateState(context);
            updateTimelineDraggingState(context, *projectData, scrollPanelBounds, mousePosition, draggedBlockBounds);
        }

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

    if(GuiButton(calculateBoundsAtAnchor(timelineAnchor, timelineBounds.addPageButton), "+")){
        navigationBarState.isAddPageRequested = true;
        navigationBarState.requestedPageInsertionIndex = projectData
            ? utilities::currentPageIndexFrom(*projectData, context.system.project.currentPage) + constants::interface_layout::timeline::FirstPageNumber
            : constants::action_center::MinimumPageIndex;
        context.interface.noteCanvas.isGridLayoutDirty = true;
    }

    navigationBarState.isPageCopyButtonPressed = GuiButton(calculateBoundsAtAnchor(timelineAnchor, timelineBounds.pageCopyButton), PageCopyButtonText);
    navigationBarState.isPagePasteButtonPressed = GuiButton(calculateBoundsAtAnchor(timelineAnchor, timelineBounds.pagePasteButton), PagePasteButtonText);
    navigationBarState.isPageCutButtonPressed = GuiButton(calculateBoundsAtAnchor(timelineAnchor, timelineBounds.pageCutButton), PageCutButtonText);
    GuiToggle(calculateBoundsAtAnchor(timelineAnchor, timelineBounds.pageSelectToggle), PageSelectToggleText, &navigationBarState.isPageSelectEnabled);

    DEBUG_PRINT_IF_CHANGED(
        "NavigationBar::drawTimeline()\n\tmaximumPageNumber={}, contentWidthInPixels={}, hoveredBlockIndex={},\n\tisTimelineDragCandidate={}, isTimelineDraggingPage={}, timelineDraggedPageIndex={}, timelineDropInsertionIndex={}, requestedPageNumber={}",
        maximumPageNumber,
        contentWidthInPixels,
        hoveredBlockIndex,
        navigationBarState.isTimelineDragCandidate,
        navigationBarState.isTimelineDraggingPage,
        navigationBarState.timelineDraggedPageIndex,
        navigationBarState.timelineDropInsertionIndex,
        navigationBarState.requestedPageNumber
    );

}
