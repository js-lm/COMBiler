#pragma once

#include "program_states/context.hpp"

namespace interface{

    class Toolbar{
    public:
        static void draw(program_states::InterfaceContext &context);

    private:
        static void drawSystemAction(program_states::InterfaceContext &context);
        static void drawStateControl(program_states::InterfaceContext &context);
        static void drawToolPalette(program_states::InterfaceContext &context);
        static void drawOption(program_states::InterfaceContext &context);
    };

} // namespace interface