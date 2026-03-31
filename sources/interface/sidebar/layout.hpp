#pragma once

#include <raylib.h>

#include "interface/anchors.hpp"

using namespace constants::anchors::sidebar;

namespace constants::layout::sidebar{

    constexpr Rectangle InspectorGroupBox   {InspectAnchor.x +  0, InspectAnchor.y +   0, 112, 408};
    constexpr Rectangle ChannelListView     {InspectAnchor.x +  8, InspectAnchor.y +  32,  96, 224};
    constexpr Rectangle ChannelLabel        {InspectAnchor.x + 32, InspectAnchor.y +   8,  48,  24};
    constexpr Rectangle StructureLine       {InspectAnchor.x +  8, InspectAnchor.y + 264,  96,  24};

    constexpr Rectangle TempoValueBox   {InspectAnchor.x + 48, InspectAnchor.y + 288, 56, 24};
    constexpr Rectangle NotesValueBox   {InspectAnchor.x + 48, InspectAnchor.y + 320, 56, 24};
    constexpr Rectangle BarsValueBox    {InspectAnchor.x + 48, InspectAnchor.y + 352, 56, 24};

} // namespace constants::layout::sidebar