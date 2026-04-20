#pragma once

#include <cstddef>

namespace constants::action_center{

    constexpr size_t MaximumHistory{500};

    // TODO: should I really put them here?
    constexpr int MaximumVolumeIndex{4};
    constexpr int MinimumVolumeIndex{0};

    constexpr int MaximumArticulationIndex{4};
    constexpr int MinimumArticulationIndex{0};

    constexpr int MinimumVolume{1};
    constexpr int MaximumVolume{10};

    constexpr int MinimumPageIndex{0};
    constexpr int FirstPageNumber{1};
    constexpr int InvalidPageInsertionIndex{-1};

} // namespace constants::action_center