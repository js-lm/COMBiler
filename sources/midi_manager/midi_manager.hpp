#pragma once

#include <cstdint>
#include <span>

#include "aliases.hpp"

class MidiManager{
public:
    MidiManager() = default;
    ~MidiManager() = default;

    void initialization();

public:
    void noteOn(const command::Target channel, const music_data::Note note);
    void noteOffconst (const command::Target channel, const music_data::Note note);

    void silence(const command::Target channel);

    void setInstrument(const command::Target channel, const music_data::Instrument instrument);

    void setTempo(const command::Tempo tempo);
    void setVolume(const command::Target channel, const units::machine::Volume volume);
    void setArticulation(const command::Target channel, const units::machine::Articulation articulation);


};