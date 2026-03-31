#pragma once

#include "program_states/context.hpp"

namespace interface{

    class NavigationBar{
    public:
        static void draw(program_states::Context &context);

    private:
        static void drawTransportControls(program_states::Context &context);
        static void drawTimeline(program_states::Context &context);
        static void drawPerPageSettings(program_states::Context &context);
        static void drawStatus(program_states::Context &context);
    };

} // namespace interface
