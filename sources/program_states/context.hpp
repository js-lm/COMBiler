#pragma once

#include "interface.hpp"
#include "system.hpp"
#include "layout.hpp"
#include "machine.hpp"

namespace program_states{

    struct InterfaceContext{
        const System &system;
        const Layout &layout;
        
        Interface   &interface;
        Machine     &machine;
    };

    struct MidiContext{
        const System &system;

        Machine &machine;
    };

    struct ReadOnlyContext{
        const System    &system;
        const Machine   &machine;
        const Layout    &layout;
        const Interface &interface;

    };

} // namespace program_states