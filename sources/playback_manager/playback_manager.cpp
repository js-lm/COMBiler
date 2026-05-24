#include "playback_manager.hpp"

#include <raylib.h>

#include "debug_utilities.hpp"

void PlaybackManager::update(MidiManager &midiManager){
    auto &machine{context_.machine};

    if(machine.isPlaying != wasPlaying_ || machine.shouldResetPlayback){
        stopPlayback(midiManager);
        if(machine.isPlaying) setupPlayback(midiManager);
        machine.shouldResetPlayback = false;
    }

    wasPlaying_ = machine.isPlaying;

    updatePlayback(midiManager);
}

void PlaybackManager::updatePlayback(MidiManager &midiManager){
    auto &machine{context_.machine};

    if(!machine.isPlaying) return;

    const float tempoPercentage{machine.tempo / static_cast<float>(constants::prompts::TempoPercentageMaximumValue)};
    const float notePerSecond{constants::midi::MaximumNotePerSecond * tempoPercentage};
    const float noteDuration{1.0f / notePerSecond};

    // DEBUG_PRINT_IF_CHANGED("tempoPercentage:{}, notePerSecond:{}, noteDuration:{}", tempoPercentage, notePerSecond, noteDuration);

    timeSinceLastNote_ += GetFrameTime();

    if(timeSinceLastNote_ >= (noteDuration * constants::midi::StaccatoReleaseDuration) 
    && !halfStepProcessed_
    ){
        releaseStaccatoNotes(midiManager);
        halfStepProcessed_ = true;
    }

    if(timeSinceLastNote_ >= noteDuration){
        timeSinceLastNote_ -= noteDuration;
        halfStepProcessed_ = false;
        nextNote(midiManager);
    }
    
}

void PlaybackManager::stopPlayback(MidiManager &midiManager){
    midiManager.silence(command::Target::Channel_1);
    midiManager.silence(command::Target::Channel_2);
    midiManager.silence(command::Target::Channel_3);
    midiManager.silence(command::Target::Channel_4);
}