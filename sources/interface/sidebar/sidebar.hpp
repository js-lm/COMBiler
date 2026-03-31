#pragma once

#include "system_states.hpp"

namespace interface{

    class Sidebar{
    private:
        SystemState &systemState_;

    public:
        Sidebar(SystemState &systemState);
        ~Sidebar() = default;

        void draw();
    };

} // namespace interface
