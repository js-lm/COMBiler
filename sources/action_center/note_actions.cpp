#include "action_center.hpp"

#include <algorithm>

#include "debug_utilities.hpp"
#include "utilities/project_utilities.hpp"

void ActionCenter::addNote(int pageNumber, int channelIndex, int noteIndex, music_data::Note note){
    updateInstrumentChannelCell(pageNumber, channelIndex, noteIndex, music_data::Note{note});

    DEBUG_PRINT(
        "addNote(); pageNumber: {}, channelIndex: {}, noteIndex: {}, note: {}", 
        pageNumber, channelIndex, noteIndex, magic_enum::enum_name<music_data::Note>(note)
    );
}

void ActionCenter::removeNote(int pageNumber, int channelIndex, int noteIndex){
    updateInstrumentChannelCell(pageNumber, channelIndex, noteIndex, std::nullopt);

    DEBUG_PRINT(
        "removeNote(); pageNumber: {}, channelIndex: {}, noteIndex: {}",
        pageNumber, channelIndex, noteIndex
    );
    
}

void ActionCenter::addInstrumentChange(int pageNumber, int channelIndex, int noteIndex, music_data::Instrument instrument){
    updateInstrumentChannelCell(pageNumber, channelIndex, noteIndex, music_data::Instrument{instrument});

    DEBUG_PRINT(
        "addInstrumentChange(); pageNumber: {}, channelIndex: {}, noteIndex: {}, instrument: {}",
        pageNumber,
        channelIndex,
        noteIndex,
        magic_enum::enum_name<music_data::Instrument>(instrument)
    );
}

void ActionCenter::copy(){
    
    
}

void ActionCenter::paste(){
    
    
}