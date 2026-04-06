#pragma once

#include <raylib.h>

namespace constants::application_window{

    constexpr int DefaultWidth{960};
    constexpr int DefaultHeight{544};

    constexpr int MinimalWidth{960};
    constexpr int MinimalHeight{544};

    constexpr int DefaultFPS{300};

    constexpr float MinimumScaleFactor{1.0f};
    constexpr float MaximumScaleFactor{4.0f};

    constexpr const char *Title{"COMBiler"};

    constexpr Vector2 Origin{.0f, .0f};

} // namespace constants::application_window