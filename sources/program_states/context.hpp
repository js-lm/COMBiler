#pragma once

#include "interface.hpp"
#include "system.hpp"
#include "layout.hpp"

namespace program_states{

    struct Context{
        const System &system;
        const Layout &layout;
        
        Interface &interface;
    };

} // namespace program_states