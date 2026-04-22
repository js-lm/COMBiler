#include "midi_manager.hpp"

void MidiManager::initialization(){

}

void MidiManager::noteOn(const command::Target channel, const music_data::Note note){
    context_.machine.activeNotes[targetChannelToVectorIndex(channel)].add(note);
}

void MidiManager::noteOff(const command::Target channel, const music_data::Note note){
    context_.machine.activeNotes[targetChannelToVectorIndex(channel)].remove(note);
}

void MidiManager::silence(const command::Target channel){

}

void MidiManager::setInstrument(const command::Target channel, const music_data::Instrument instrument){
    context_.machine.instruments[targetChannelToVectorIndex(channel)] = instrument;
}

void MidiManager::setTempo(const command::Tempo tempo){
    context_.machine.tempo = tempo;
}

void MidiManager::setVolume(const command::Target channel, const units::machine::Volume volume){
    if(channel == command::Target::All_Channels){
        for(auto &targetVolume : context_.machine.volumes){
            targetVolume = volume;
        }
    }else{
        context_.machine.volumes[targetChannelToVectorIndex(channel)] = volume;
    }
}

void MidiManager::setArticulation(const command::Target channel, const units::machine::Articulation articulation){
    if(channel == command::Target::All_Channels){
        for(auto &targetArticulation : context_.machine.articulations){
            targetArticulation = articulation;
        }
    }else{
        context_.machine.articulations[targetChannelToVectorIndex(channel)] = articulation;
    }
}