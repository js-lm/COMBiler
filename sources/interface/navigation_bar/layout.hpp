#pragma once

#include <raylib.h>

#include "interface/anchors.hpp"

using namespace constants::anchors::navigation_bar;

namespace constants::layout::navigation_bar{

    constexpr Rectangle TransportControlsGroupBox   {TransportControlsAnchor.x +   0, TransportControlsAnchor.y + 0, 232, 40};
    constexpr Rectangle PreviousPageButton          {TransportControlsAnchor.x +  32, TransportControlsAnchor.y + 8,  24, 24};
    constexpr Rectangle NextPageButton              {TransportControlsAnchor.x +  60, TransportControlsAnchor.y + 8,  24, 24};
    constexpr Rectangle StopButton                  {TransportControlsAnchor.x + 130, TransportControlsAnchor.y + 8,  24, 24};
    constexpr Rectangle PlayButton                  {TransportControlsAnchor.x + 102, TransportControlsAnchor.y + 8,  24, 24};
    constexpr Rectangle PageRepeatToggle            {TransportControlsAnchor.x + 176, TransportControlsAnchor.y + 8,  24, 24};

    constexpr Rectangle TimelineGroupBox    {TimelineAnchor.x +   0, TimelineAnchor.y +  0, 960, 96};
    constexpr Rectangle TimelineScrollPanel {TimelineAnchor.x + 112, TimelineAnchor.y + 24, 776, 72};
    constexpr Rectangle TimelineLabel       {TimelineAnchor.x + 112, TimelineAnchor.y +  0, 120, 24};
    constexpr Rectangle PageNumberLabel     {TimelineAnchor.x + 816, TimelineAnchor.y +  0,  72, 24};
    constexpr Rectangle PageCopyButton      {TimelineAnchor.x + 896, TimelineAnchor.y + 24,  24, 24};
    constexpr Rectangle PagePasteButton     {TimelineAnchor.x + 928, TimelineAnchor.y + 24,  24, 24};
    constexpr Rectangle PageCutButton       {TimelineAnchor.x + 928, TimelineAnchor.y + 56,  24, 24};
    constexpr Rectangle PageSelectToggle    {TimelineAnchor.x + 896, TimelineAnchor.y + 56,  24, 24};

    constexpr Rectangle PerPageSettingsGroupBox {PerPageSettingsAnchor.x +   0, PerPageSettingsAnchor.y + 0, 312, 40};
    constexpr Rectangle NotePerPageSpinner      {PerPageSettingsAnchor.x + 208, PerPageSettingsAnchor.y + 8,  88, 24};

    constexpr Rectangle StatusGroupBox{StatusAnchor.x + 0, StatusAnchor.y + 0, 304, 40};

} // namespace constants::layout::sidebar