#pragma once

#include "system_states.hpp"

namespace interface{

    class NavigationBar{
    public:
        static void draw(SystemState &systemState);

    private:
        static void drawTransportControls(SystemState &systemState);
        static void drawTimeline(SystemState &systemState);
        static void drawPerPageSettings(SystemState &systemState);
        static void drawStatus(SystemState &systemState);
    };

} // namespace interface
