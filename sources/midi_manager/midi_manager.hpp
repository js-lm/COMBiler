#pragma once

#include <cstdint>
#include <vector>
#include <mutex>

#include <raylib.h>

#include <tsf.h>

#include "aliases.hpp"

#include "constants.hpp"

#include "program_states/context.hpp"
#include "program_states/system.hpp"

#include "command/command_type.hpp"

class MidiManager{
public:
    inline static tsf *soundFont{nullptr};
    inline static std::mutex audioMutex{};

private:
    program_states::MidiContext context_;

private:
    AudioStream outputAudioStream_{};

    bool hasInitializedAudioDevice_{false};

    int outputSampleRate_{constants::midi::DefaultOutputSampleRate};
    int streamChunkFrameCount_{constants::midi::StreamChunkFrameCount};

    // std::vector<int16_t> interleavedSampleBuffer_{};

public:
    MidiManager(program_states::MidiContext context)
        : context_{context}
    {}
    MidiManager() = delete;
    ~MidiManager();

    program_states::System::InitializationError initialization();
    
    void update();

public:
    void noteOn(const command::Target channel, const music_data::Note note);
    void noteOff(const command::Target channel, const music_data::Note note);

    void silence(const command::Target channel);

    void setInstrument(const command::Target channel, const music_data::Instrument instrument);

    void setTempo(const command::Tempo tempo);
    void setVolume(const command::Target channel, const units::machine::Volume volume);
    void setArticulation(const command::Target channel, const units::machine::Articulation articulation);

private:
    void activateNote(const units::midi::SoundFontChannel channel, const music_data::Note note);
    void deactivateNote(const units::midi::SoundFontChannel channel, const music_data::Note note);
    void silenceChannel(const units::midi::SoundFontChannel channel);

    void applyInstrument(const units::midi::SoundFontChannel channel, const music_data::Instrument instrument);
    void applyVolume(const units::midi::SoundFontChannel channel);
    void applyArticulation(const units::midi::SoundFontChannel channel);

    int instrumentChannel(const units::midi::SoundFontChannel channel) const;
    float normalizedVolume(const units::machine::Volume volume) const;

private:
    int targetChannelToVectorIndex(const command::Target channel) const;
};