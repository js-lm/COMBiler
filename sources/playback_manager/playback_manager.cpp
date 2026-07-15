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
    static int lastKnownUIPage{context_.system.project.currentPage};

    /* UI -> Sequencer */ {
        std::lock_guard<std::mutex> lock(stateMutex_);
        
        if(machine.isPlaying && !wasPlaying_){
            threadState_.uiRequestedPlay = true;
            machine.playheadIndex = 0;
        }else if(!machine.isPlaying && wasPlaying_){
            threadState_.uiRequestedStop = true;
        }
        
        if(machine.shouldResetPlayback){
            threadState_.uiRequestedReset = true;
            machine.playheadIndex = 0;
            machine.shouldResetPlayback = false;
        }

        threadState_.internalIsPageRepeatEnabled = context_.interface.navigationBar.isPageRepeatEnabled;

        if(context_.system.project.currentPage != lastKnownUIPage){
            threadState_.uiRequestedPageChange = context_.system.project.currentPage;
        }
        lastKnownUIPage = context_.system.project.currentPage;
    } /* UI -> Sequencer */

    /* Sequencer -> UI */ {
        std::lock_guard<std::mutex> lock(stateMutex_);
        
        if(!threadState_.uiRequestedPlay && !threadState_.uiRequestedReset){
            machine.playheadIndex = threadState_.internalMachine.playheadIndex;
        }
        
        bool justStopped{wasPlaying_ && !threadState_.internalMachine.isPlaying};
        if(justStopped){
            machine.isPlaying = false;
        }
        wasPlaying_ = machine.isPlaying;

        if(threadState_.internalCurrentPage != context_.system.project.currentPage){
            context_.interface.navigationBar.requestedPageNumber = threadState_.internalCurrentPage;
            lastKnownUIPage = threadState_.internalCurrentPage;
        }
        
        if(machine.isPlaying || justStopped){
            machine.activeNotes = threadState_.internalMachine.activeNotes;
        }
    } /* Sequencer -> UI */
}

void PlaybackManager::sequencerLoop(MidiManager &midiManager){
    auto lastTime{std::chrono::high_resolution_clock::now()};

    while(!shouldStopThread_){
        auto currentTime{std::chrono::high_resolution_clock::now()};
        float deltaTime{std::chrono::duration<float>(currentTime - lastTime).count()};
        lastTime = currentTime;

        /* process incoming ui requests */ {
            std::lock_guard<std::mutex> lock(stateMutex_);
            if(threadState_.uiRequestedPlay){
                threadState_.internalMachine.isPlaying = true;
                setupPlayback(midiManager);
                threadState_.uiRequestedPlay = false;
            }
            if(threadState_.uiRequestedStop){
                threadState_.internalMachine.isPlaying = false;
                stopPlayback(midiManager);
                threadState_.uiRequestedStop = false;
            }
            if(threadState_.uiRequestedReset){
                stopPlayback(midiManager);
                setupPlayback(midiManager);
                threadState_.uiRequestedReset = false;
            }
            if(threadState_.uiRequestedPageChange.has_value()){
                threadState_.internalCurrentPage = threadState_.uiRequestedPageChange.value();
                threadState_.uiRequestedPageChange = std::nullopt;
                if(threadState_.internalMachine.isPlaying){
                    stopPlayback(midiManager);
                    setupPlayback(midiManager);
                }
            }
        } /* process incoming ui requests */

        updatePlayback(midiManager, deltaTime);

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void PlaybackManager::updatePlayback(MidiManager &midiManager, float deltaTime){
    auto &machine{threadState_.internalMachine};

    if(!machine.isPlaying) return;

    const auto projectData{utilities::projectDataFrom(context_.system)};
    float maximumTempo{constants::project_data::DefaultMaximumTempo};
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

    for(auto &notesSet : threadState_.internalMachine.activeNotes){
        notesSet.clear();
    }
}