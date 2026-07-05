#include "playback_manager.hpp"

#include <raylib.h>

#include "debug_utilities.hpp"

#include "utilities/project_utilities.hpp"

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

    const auto projectData{utilities::projectDataFrom(context_.system)};
    float maximumTempo{12.0f};
    if(projectData) maximumTempo = projectData->metadata.maximumTempo;

    const float tempoPercentage{machine.tempo / static_cast<float>(constants::prompts::TempoPercentageMaximumValue)};
    const float notePerSecond{maximumTempo * tempoPercentage};
    const float noteDuration{1.0f / notePerSecond};

    // DEBUG_PRINT_IF_CHANGED("tempoPercentage:{}, notePerSecond:{}, noteDuration:{}", tempoPercentage, notePerSecond, noteDuration);

    if(needsToPlayCurrentNote_){
        playCurrentNote(midiManager);
        needsToPlayCurrentNote_ = false;
        halfStepProcessed_ = false;
    }

    timeSinceLastNote_ += GetFrameTime();

    if(timeSinceLastNote_ >= (noteDuration * constants::midi::StaccatoReleaseDuration) 
    && !halfStepProcessed_
    ){
        releaseStaccatoNotes(midiManager);
        halfStepProcessed_ = true;
    }

    if(timeSinceLastNote_ >= noteDuration){
        timeSinceLastNote_ -= noteDuration;
        advancePlayhead(midiManager);
    }
    
}

void PlaybackManager::stopPlayback(MidiManager &midiManager){
    midiManager.silence(command::Target::Channel_1);
    midiManager.silence(command::Target::Channel_2);
    midiManager.silence(command::Target::Channel_3);
    midiManager.silence(command::Target::Channel_4);
}