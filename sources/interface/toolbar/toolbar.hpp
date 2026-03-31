#pragma once

#include "program_states/context.hpp"

namespace interface{

    class Toolbar{
    public:
        static void draw(program_states::Context &context);

    private:
        static void drawSystemAction(program_states::Context &context);
        static void drawStateControl(program_states::Context &context);
        static void drawToolPalette(program_states::Context &context);
        static void drawOption(program_states::Context &context);
    };

} // namespace interface