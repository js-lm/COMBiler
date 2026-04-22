#pragma once

#include <cstdint>
#include <span>

#include "aliases.hpp"

#include "program_states/context.hpp"

class MidiManager{
private:
    program_states::MidiContext context_;

public:
    MidiManager(program_states::MidiContext context)
        : context_{context}
    {}
    MidiManager() = delete;
    ~MidiManager() = default;

    void initialization();

public:
    void noteOn(const command::Target channel, const music_data::Note note);
    void noteOff(const command::Target channel, const music_data::Note note);

    void silence(const command::Target channel);

    void setInstrument(const command::Target channel, const music_data::Instrument instrument);

    void setTempo(const command::Tempo tempo);
    void setVolume(const command::Target channel, const units::machine::Volume volume);
    void setArticulation(const command::Target channel, const units::machine::Articulation articulation);

private:
    int targetChannelToVectorIndex(const command::Target channel) const;
};