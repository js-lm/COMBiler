#pragma once

#include "program_states/context.hpp"

namespace interface{

    class NoteCanvas{
    public:
        static void draw(program_states::Context &context);
    };

} // namespace interface
