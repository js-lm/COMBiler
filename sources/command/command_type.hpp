#pragma once

#include "aliases.hpp"

#include <array>
#include <unordered_map>

#include "constants.hpp"

namespace command{

    enum class Target : uint8_t{
        All_Channels,
        Channel_1,
        Channel_2,
        Channel_3,
        Channel_4
    };

    struct Tempo{
        units::machine::Tempo tempo{constants::midi::DefaultTempo};
    };

    struct Volume{
        units::machine::Volume volume{constants::midi::DefaultVolume};
        Target target{Target::All_Channels};
    };

    struct Articulation{
        units::machine::Articulation articulation{constants::midi::DefaultArticulation};
        Target target{Target::All_Channels};
    };

    using Command = std::variant<Tempo, Volume, Articulation>;

    using ConstantIndex = uint8_t;

    using CommandToken = std::variant<Command, ConstantIndex>;

} // namespace commands