#pragma once

#include "system_states.hpp"

namespace interface{

    class NoteCanvas{
    public:
        static void draw(SystemState &systemState);
    };

} // namespace interface
