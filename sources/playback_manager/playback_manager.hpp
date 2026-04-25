#pragma once

#include "program_states/context.hpp"

class PlaybackManager{
private:
    program_states::MidiContext context_;

private:
    bool wasPlaying_{false};

public:
    PlaybackManager(program_states::MidiContext context)
        : context_{context}
    {}
    PlaybackManager() = delete;
    ~PlaybackManager() = default;

    void initialization();

    void update();

private:
    void setupPlayback();

    void resetCachedMachineState();
};