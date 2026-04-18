#pragma once

#include "program_states/context.hpp"

class TimelineManager;

namespace interface{

    class NavigationBar{
    public:
        static void draw(program_states::InterfaceContext &context);
        static void bindTimelineManager(const TimelineManager *timelineManager);

    private:
        static const TimelineManager *timelineManager_;

    private:
        static void drawTransportControls(program_states::InterfaceContext &context);
        static void drawTimeline(program_states::InterfaceContext &context);
        static void drawPerPageSettings(program_states::InterfaceContext &context);
        static void drawStatus(program_states::InterfaceContext &context);

    private:
        static float timelineBlockWidthInPixels(
            const program_states::ProjectData &projectData,
            size_t pageIndex
        );
        static float timelineContentWidthInPixels(const program_states::ProjectData &projectData);
        static float drawTimelineBlocksAndMarkers(
            program_states::InterfaceContext    &context,
            const program_states::ProjectData   &projectData,
            const Rectangle                     &scrollPanelBounds,
            const Vector2                       &mousePosition,
            bool                                isMouseInsideScrollPanel,
            int                                 &hoveredBlockIndex,
            int                                 &hoveredMarkerIndex,
            Rectangle                           &draggedBlockBounds
        );
        static void handleTimelineMousePress(
            program_states::InterfaceContext    &context,
            program_states::ProjectData         &projectData,
            const Rectangle                     &scrollPanelBounds,
            const Vector2                       &mousePosition,
            bool                                isMouseInsideScrollPanel,
            int                                 hoveredBlockIndex,
            int                                 hoveredMarkerIndex
        );
        static void updateTimelineDragCandidateState(
            program_states::InterfaceContext &context,
            int hoveredMarkerIndex
        );
        static void updateTimelineDraggingState(
            program_states::InterfaceContext    &context,
            program_states::ProjectData         &projectData,
            int                                 hoveredMarkerIndex,
            const Rectangle                     &draggedBlockBounds
        );
    };

} // namespace interface
