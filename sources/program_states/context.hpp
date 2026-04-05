#pragma once

#include "interface.hpp"
#include "system.hpp"
#include "layout.hpp"

namespace program_states{

    struct InterfaceContext{
        const System &system;
        const Layout &layout;
        
        Interface &interface;
    };

    struct ReadOnlyContext{
        const System &system;
        const Layout &layout;
        
        const Interface &interface;
    };

} // namespace program_states