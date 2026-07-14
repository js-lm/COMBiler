#include "playback_manager.hpp"

#include <raylib.h>
#include <chrono>

#include "debug_utilities.hpp"

#include "utilities/project_utilities.hpp"

PlaybackManager::PlaybackManager(program_states::MidiContext context, MidiManager &midiManager)
    : context_{context}
{
    threadState_.internalCurrentPage = context.system.project.currentPage;
    sequencerThread_ = std::thread(&PlaybackManager::sequencerLoop, this, std::ref(midiManager));
}

PlaybackManager::~PlaybackManager(){
    shouldStopThread_ = true;
    if(sequencerThread_.joinable()){
        sequencerThread_.join();
    }
}

void PlaybackManager::initialization(){

}

void PlaybackManager::update(MidiManager &midiManager){
    auto &machine{context_.machine};

    /* UI -> Sequencer */ {
        std::lock_guard<std::mutex> lock(stateMutex_);
        

    } /* UI -> Sequencer */

    /* Sequencer -> UI */ {
        std::lock_guard<std::mutex> lock(stateMutex_);

    } /* Sequencer -> UI */
}

void PlaybackManager::sequencerLoop(MidiManager &midiManager){

}

void PlaybackManager::updatePlayback(MidiManager &midiManager, float deltaTime){
    auto &machine{threadState_.internalMachine};

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

    timeSinceLastNote_ += deltaTime;

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