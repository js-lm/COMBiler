#include "action_center.hpp"

#include <algorithm>

#include "debug_utilities.hpp"

void ActionCenter::addNote(int pageNumber, int channelIndex, int noteIndex, music_data::Note note){
    
    auto &page{stagedSlot_->data->pages[pageNumber - 1]};

    beginAction();

    stagedSlot_->data->transient.currentPageNumber = pageNumber;
    stagedSlot_->data->transient.selectedChannelListViewIndex = channelIndex + 1;

    page.instrumentChannels[channelIndex][noteIndex] = music_data::Note{note};

    DEBUG_PRINT(
        "pageNumber: {}, channelIndex: {}, noteIndex: {}, note: {}", 
        pageNumber, channelIndex, noteIndex, magic_enum::enum_name<music_data::Note>(note)
    );
}

void ActionCenter::removeNote(int pageNumber, int noteIndex){

    
}

void ActionCenter::copyNote(){
    
    
}

void ActionCenter::pasteNote(){
    
    
}