#pragma once

#include <raylib.h>

#include <memory>

#include "project_data.hpp"

#include "action_center/action_center.hpp"

namespace program_states{

    struct System{
        enum class InitializationError{
            None,
            SoundFontMissing,
            SoundFontLoadFailed
        };
        InitializationError initializationError{InitializationError::None};

        struct Window{
            int interfaceRenderTextureWidth{0};
            int interfaceRenderTextureHeight{0};

            float scaleFactor{1.0f};

            bool isAlwaysScalingToMaximum{true};
            bool isIntegerScaling{true};

            bool isLayoutDirty{false};
        } window{};

        struct Project{

            int currentPage{1}; // 1 base

            std::weak_ptr<ActionCenter::StagedSlot> data{};

        } project{};

        struct NoteCanvas{
            Camera2D gridCamera{
                .offset     {.0f, .0f},
                .target     {.0f, .0f},
                .rotation   {.0f},
                .zoom       {1.0f}
            };
        } noteCanvas{};

        struct Audio{
            float masterVolume{1.0f};
        } audio{};

        struct Performance{
            bool limitFpsTo60{false};
            bool idleDuringEventWaiting{false};
        } performance{};

        bool isConfigurationDirty{false};
    };

} // namespace program_states