#pragma once

#include "program_states/context.hpp"

#include "midi_manager/midi_manager.hpp"

#include "command/command_type.hpp"

#include <thread>
#include <mutex>
#include <atomic>

class PlaybackManager{
private:
    program_states::MidiContext context_;

private:
    bool wasPlaying_{false};

    float timeSinceLastNote_{.0f};

    bool halfStepProcessed_{false};
    bool needsToPlayCurrentNote_{false};

    struct ThreadState{
        program_states::Machine internalMachine{};
        int internalCurrentPage{1};
        bool internalIsPageRepeatEnabled{false};
        
        bool uiRequestedPlay{false};
        bool uiRequestedStop{false};
        bool uiRequestedReset{false};
        std::optional<int> uiRequestedPageChange{};
        std::optional<float> uiRequestedTempo{};
    } threadState_{};

    std::mutex stateMutex_{};
    std::thread sequencerThread_{};
    std::atomic<bool> shouldStopThread_{false};

public:
    PlaybackManager(program_states::MidiContext context, MidiManager &midiManager);
    PlaybackManager() = delete;
    ~PlaybackManager();

    void initialization();

    void update(MidiManager &midiManager);

private:
    void setupPlayback(MidiManager &midiManager);
    void updatePlayback(MidiManager &midiManager, float deltaTime);
    void stopPlayback(MidiManager &midiManager);

    void playCurrentNote(MidiManager &midiManager);
    void advancePlayhead(MidiManager &midiManager);

    void releaseStaccatoNotes(MidiManager &midiManager);

    void sequencerLoop(MidiManager &midiManager);
    
private:
    void updateNoteState(
        units::midi::SoundFontChannel channel, 
        std::optional<music_data::Note> currentNote, 
        MidiManager &midiManager
    );

private:
    command::Target channelIndexToChannelTarget(units::midi::SoundFontChannel channel);
};