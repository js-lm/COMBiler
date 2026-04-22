#pragma once

#include "aliases.hpp"

#include "constants.hpp"

#include <array>
#include <vector>
#include <unordered_set>

#include <enum_set.hpp>

namespace program_states{

    struct Machine{
        bool isPlaying{false};

        command::Tempo tempo{};
        
        std::array<music_data::Instrument, constants::project_data::NumberOfInstrumentChannels> instruments{};
        std::array<utilities::EnumSet<music_data::Note>, constants::project_data::NumberOfInstrumentChannels> onNotes{};
        
        std::array<command::Volume, constants::project_data::NumberOfInstrumentChannels> volumes{};
        std::array<command::Articulation, constants::project_data::NumberOfInstrumentChannels> articulations{};

    };

} // namespace program_states