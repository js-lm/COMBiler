#pragma once

#include <raylib.h>

#include <memory>

#include "project_data.hpp"

namespace program_states{

    struct System{

        struct Window{
            int interfaceRenderTextureWidth{0};
            int interfaceRenderTextureHeight{0};

            float scaleFactor{1.0f};

            bool isAlwaysScalingToMaximum{true};
            bool isIntegerScaling{true};
        } window{};

        struct Project{

            int currentPage{1};

            // std::unique_ptr<ProjectData> data{&DEBUG_preset::projectData};

        } project{};

        struct NoteCanvas{
            Camera2D gridCamera{
                .offset     {.0f, .0f},
                .target     {.0f, .0f},
                .rotation   {.0f},
                .zoom       {1.0f}
            };
        } noteCanvas{};



    };

} // namespace program_states