#include "midi_manager.hpp"

int MidiManager::targetChannelToVectorIndex(const command::Target channel) const{
    switch(channel){
    case command::Target::Channel_1: return 0;
    case command::Target::Channel_2: return 1;
    case command::Target::Channel_3: return 2;
    case command::Target::Channel_4: return 3;
    default: return 0;
    }
}

void MidiManager::activateNote(const units::midi::SoundFontChannel channel, const music_data::Note note){
    if(!context_.machine.activeNotes[channel].add(note)) return;

    const int soundFontChannel{instrumentChannel(channel)};
    const int midiNoteNumber{static_cast<int>(note)};
    const float velocity{normalizedVolume(context_.machine.volumes[channel])};

    std::lock_guard<std::mutex> lock(MidiManager::audioMutex);
    tsf_channel_note_on(soundFont, soundFontChannel, midiNoteNumber, velocity);
}

void MidiManager::deactivateNote(const units::midi::SoundFontChannel channel, const music_data::Note note){
    if(!context_.machine.activeNotes[channel].remove(note)) return;

    // const units::machine::Articulation articulation{context_.machine.articulations[channel]};

    const int soundFontChannel{instrumentChannel(channel)};
    const int midiNoteNumber{static_cast<int>(note)};

    std::lock_guard<std::mutex> lock(MidiManager::audioMutex);
    tsf_channel_note_off(soundFont, soundFontChannel, midiNoteNumber);
}

void MidiManager::silenceChannel(const units::midi::SoundFontChannel channel){
    context_.machine.activeNotes[channel].clear();

    const int soundFontChannel{instrumentChannel(channel)};

    std::lock_guard<std::mutex> lock(MidiManager::audioMutex);
    tsf_channel_note_off_all(soundFont, soundFontChannel);
    tsf_channel_sounds_off_all(soundFont, soundFontChannel);
}

void MidiManager::applyInstrument(const units::midi::SoundFontChannel channel, const music_data::Instrument instrument){
    const int instrumentChannelIndex{channel};
    context_.machine.instruments[instrumentChannelIndex] = instrument;

    const int soundFontChannel{instrumentChannel(channel)};
    const units::midi::SoundPresetID soundPresetID{constants::midi::InstrumentsMap[instrument]};

    const bool isDrumInstrument{instrument == music_data::Instrument::Drum_Sets};
    const int soundPresetNumber{isDrumInstrument ? constants::midi::DrumSoundPresetNumber : soundPresetID};

    std::lock_guard<std::mutex> lock(MidiManager::audioMutex);
    tsf_channel_set_presetnumber(soundFont, soundFontChannel, soundPresetNumber, isDrumInstrument ? 1 : 0);
}

void MidiManager::applyVolume(const units::midi::SoundFontChannel channel){
    const int soundFontChannel{instrumentChannel(channel)};
    const float volume{normalizedVolume(context_.machine.volumes[channel])};

    std::lock_guard<std::mutex> lock(MidiManager::audioMutex);
    tsf_channel_set_volume(soundFont, soundFontChannel, volume);
}

void MidiManager::applyArticulation(const units::midi::SoundFontChannel channel){
    // TODO
}

int MidiManager::instrumentChannel(const units::midi::SoundFontChannel channel) const{
    const music_data::Instrument instrument{context_.machine.instruments[channel]};

    if(instrument == music_data::Instrument::Drum_Sets){
        return constants::midi::DrumSoundFontChannel;
    }

    return channel;
}

float MidiManager::normalizedVolume(const units::machine::Volume volume) const{
    return static_cast<float>(volume) / constants::action_center::MaximumVolume;
}