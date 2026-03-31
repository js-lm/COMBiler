#pragma once

#include "system_states.hpp"

namespace interface{

    class Sidebar{
    public:
        static void draw(SystemState &systemState);

    private:
        static void drawInspector(SystemState &systemState);
    };

} // namespace interface
