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
    };

} // namespace interface
