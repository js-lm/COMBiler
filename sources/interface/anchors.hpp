#pragma once

#include <raylib.h>

namespace constants::anchors{

    namespace toolbar{
        constexpr Vector2 SystemActionsAnchor   {  0, 0};
        constexpr Vector2 StateControlAnchor    {248, 0};
        constexpr Vector2 ToolPaletteAnchor     {448, 0};
        constexpr Vector2 ToolOptionsAnchor     {784, 0};
    }

    namespace sidebar{
        constexpr Vector2 InspectAnchor{0, 40};
    }

    namespace navigation_bar{
        constexpr Vector2 TransportControlsAnchor   {416, 408};
        constexpr Vector2 PerPageSettingsAnchor     {648, 408};
        constexpr Vector2 TimelineAnchor            {  0, 448};
        constexpr Vector2 StatusAnchor              {112, 408};
    }

    namespace note_canvas{
        constexpr Vector2 NoteCanvasAnchor{112, 40};
    }



} // constants::anchors