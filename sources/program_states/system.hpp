#pragma once

#include <raylib.h>

namespace program_states{

    struct System{

        struct Window{
            int interfaceRenderTextureWidth{0};
            int interfaceRenderTextureHeight{0};

            float scaleFactor{1.0f};

            bool isAlwaysScalingToMaximum{false};
            bool isIntegerScaling{false};
        } window{};





    };

} // namespace program_states