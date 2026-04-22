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