#pragma once

#include <raylib.h>

namespace constants::prompts{

    enum class CommandPrompt{
        Tempo,
        Volume,
        Articulation
    };

    constexpr int CommandToolCount{3};
    constexpr int OpenCommandPromptButtonToolIndex{3};

    constexpr int toIndex(CommandPrompt selectedPrompt){
        return static_cast<int>(selectedPrompt);
    }

    inline CommandPrompt commandPromptFromIndex(int commandPromptIndex){
        switch(commandPromptIndex){
        case toIndex(CommandPrompt::Tempo): return CommandPrompt::Tempo;
        case toIndex(CommandPrompt::Volume): return CommandPrompt::Volume;
        default: return CommandPrompt::Articulation;
        }
    }

    constexpr int TempoPercentageMinimumValue{2};
    constexpr int TempoPercentageMaximumValue{100};
    constexpr float OverlayDimAlpha{.35f};

} // namespace constants::prompts
