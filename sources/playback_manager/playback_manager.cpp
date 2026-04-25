#include "playback_manager.hpp"

#include <raylib.h>

#include "debug_utilities.hpp"

void PlaybackManager::update(MidiManager &midiManager){
    auto &machine{context_.machine};

    if(machine.isPlaying && machine.isPlaying != wasPlaying_){
        // machine.reset();
        // timeSinceLastNote_ = .0f;
        // machine.playheadIndex = 0;
        setupPlayback();
    }

    wasPlaying_ = machine.isPlaying;

    if(!machine.isPlaying) return;


    const float tempoPercentage{machine.tempo / 100.0f};
    const float notePerSecond{constants::midi::MaximumNotePerSecond * tempoPercentage};
    const float noteDuration{1.0f / notePerSecond};

    DEBUG_PRINT_IF_CHANGED("tempoPercentage:{}, notePerSecond:{}, noteDuration:{}", tempoPercentage, notePerSecond, noteDuration);

    if(timeSinceLastNote_ >= noteDuration){
        timeSinceLastNote_ -= noteDuration;
        nextNote(midiManager);
    }else{
        timeSinceLastNote_ += GetFrameTime();
    }

    
}