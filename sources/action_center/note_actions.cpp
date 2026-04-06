#include "action_center.hpp"

#include <algorithm>

#include "debug_utilities.hpp"
#include "utilities/project_utilities.hpp"

void ActionCenter::addNote(int pageNumber, int channelIndex, int noteIndex, music_data::Note note){
    
    auto &page{utilities::pageByNumber(*stagedSlot_->data, pageNumber)};

    beginAction();

    utilities::applyTransientSelection(stagedSlot_->data->transient, pageNumber, channelIndex);

    page.instrumentChannels[channelIndex][noteIndex] = music_data::Note{note};

    DEBUG_PRINT(
        "addNote(); pageNumber: {}, channelIndex: {}, noteIndex: {}, note: {}", 
        pageNumber, channelIndex, noteIndex, magic_enum::enum_name<music_data::Note>(note)
    );
}

void ActionCenter::removeNote(int pageNumber, int channelIndex, int noteIndex){

    auto &page{utilities::pageByNumber(*stagedSlot_->data, pageNumber)};

    beginAction();

    utilities::applyTransientSelection(stagedSlot_->data->transient, pageNumber, channelIndex);

    page.instrumentChannels[channelIndex][noteIndex] = std::nullopt;

    DEBUG_PRINT(
        "removeNote(); pageNumber: {}, channelIndex: {}, noteIndex: {}",
        pageNumber, channelIndex, noteIndex
    );
    
}

void ActionCenter::copyNote(){
    
    
}

void ActionCenter::pasteNote(){
    
    
}