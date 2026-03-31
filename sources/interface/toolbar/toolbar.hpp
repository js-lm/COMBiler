#pragma once

#include "system_states.hpp"

namespace interface{

    class Toolbar{
    // private:
    //     SystemState &systemState_;

    public:
        // Toolbar(SystemState &systemState);
        // ~Toolbar() = default;

        static void draw(SystemState &systemState);

    private:
        static void drawSystemAction(SystemState &systemState);
        static void drawStateControl(SystemState &systemState);
        static void drawToolPalette(SystemState &systemState);
        static void drawOption(SystemState &systemState);
    };

} // namespace interface