#include "playback_manager.hpp"

#include <raylib.h>

#include "debug_utilities.hpp"

void PlaybackManager::update(MidiManager &midiManager){
    auto &machine{context_.machine};

    if(machine.isPlaying != wasPlaying_){
        if(machine.isPlaying) setupPlayback();
        else stopPlayback(midiManager);
    }

    wasPlaying_ = machine.isPlaying;

    updatePlayback(midiManager);
}

void PlaybackManager::updatePlayback(MidiManager &midiManager){
    auto &machine{context_.machine};

    if(!machine.isPlaying) return;

    const float tempoPercentage{machine.tempo / 100.0f};
    const float notePerSecond{constants::midi::MaximumNotePerSecond * tempoPercentage};
    const float noteDuration{1.0f / notePerSecond};

    // DEBUG_PRINT_IF_CHANGED("tempoPercentage:{}, notePerSecond:{}, noteDuration:{}", tempoPercentage, notePerSecond, noteDuration);

    if(timeSinceLastNote_ >= noteDuration){
        timeSinceLastNote_ -= noteDuration;
        nextNote(midiManager);
    }else{
        timeSinceLastNote_ += GetFrameTime();
    }
    
}

void PlaybackManager::stopPlayback(MidiManager &midiManager){
    midiManager.silence(command::Target::Channel_1);
    midiManager.silence(command::Target::Channel_2);
    midiManager.silence(command::Target::Channel_3);
    midiManager.silence(command::Target::Channel_4);
}