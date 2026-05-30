#pragma once

#include "program_states/context.hpp"

#include "midi_manager/midi_manager.hpp"

#include "command/command_type.hpp"

class PlaybackManager{
private:
    program_states::MidiContext context_;

private:
    bool wasPlaying_{false};

    float timeSinceLastNote_{.0f};

    bool halfStepProcessed_{false};
    bool needsToPlayCurrentNote_{false};

public:
    PlaybackManager(program_states::MidiContext context)
        : context_{context}
    {}
    PlaybackManager() = delete;
    ~PlaybackManager() = default;

    void initialization();

    void update(MidiManager &midiManager);

private:
    void setupPlayback(MidiManager &midiManager);
    void updatePlayback(MidiManager &midiManager);
    void stopPlayback(MidiManager &midiManager);

    void playCurrentNote(MidiManager &midiManager);
    void advancePlayhead(MidiManager &midiManager);

    void releaseStaccatoNotes(MidiManager &midiManager);
    
private:
    void updateNoteState(
        units::midi::SoundFontChannel channel, 
        std::optional<music_data::Note> currentNote, 
        MidiManager &midiManager
    );

private:
    command::Target channelIndexToChannelTarget(units::midi::SoundFontChannel channel);
};