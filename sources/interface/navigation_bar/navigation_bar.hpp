#pragma once

#include "system_states.hpp"

namespace interface{

    class NavigationBar{
    private:
        SystemState &systemState_;

    public:
        NavigationBar(SystemState &systemState);
        ~NavigationBar() = default;

        void draw();
    };

} // namespace interface
