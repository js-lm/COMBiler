#pragma once

#include "system_states.hpp"

namespace interface{

    class NoteCanvas{
    private:
        SystemState &systemState_;

    public:
        NoteCanvas(SystemState &systemState);
        ~NoteCanvas() = default;

        void draw();
    };

} // namespace interface
