#pragma once

#include <raylib.h>

namespace constants::application_window{

    constexpr int DefaultWidth{960};
    constexpr int DefaultHeight{544};

    constexpr int MinimalWidth{960};
    constexpr int MinimalHeight{544};

    constexpr int ErrorWindowWidth{800};
    constexpr int ErrorWindowHeight{150};
    constexpr int ErrorWindowMargin{10};

    constexpr int DefaultFPS{300};

    constexpr const char *ErrorMessageSoundFontMissing{"Please place a sound font (.sf2) in the soundfonts/ directory."};
    constexpr const char *ErrorMessageSoundFontLoadError{"Error loading the sound font file."};

    constexpr float MinimumScaleFactor{1.0f};
    constexpr float MaximumScaleFactor{4.0f};

    constexpr const char *Title{"COMBiler v" COMBILER_VERSION};

    constexpr Vector2 Origin{.0f, .0f};

} // namespace constants::application_window