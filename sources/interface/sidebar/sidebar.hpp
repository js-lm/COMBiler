#pragma once

#include "program_states/context.hpp"

namespace interface{

    class Sidebar{
    public:
        static void draw(program_states::InterfaceContext &context);

    private:
        static void drawInspector(program_states::InterfaceContext &context);
    };

} // namespace interface
