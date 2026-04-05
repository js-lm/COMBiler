#include "action_center.hpp"

#include "debug_utilities.hpp"

void ActionCenter::commit(){
    const size_t maximumSize{constants::action_center::MaximumHistory};

    if(historyStackSize_ == 0){
        commits_[0] = std::move(stagedSlot_->data);
        historyTail_ = 0;
        historyHead_ = 0;
        cursorIndex_ = 0;
        historyStackSize_ = 1;

        return;
    }

    size_t nextIndex{(cursorIndex_ + 1) % maximumSize};
    
    bool wasFull{historyStackSize_ == maximumSize};
    bool isAdvancingTail{wasFull && (nextIndex == historyTail_ || cursorIndex_ == historyHead_)};

    commits_[nextIndex] = std::move(stagedSlot_->data);
    cursorIndex_ = nextIndex;
    historyHead_ = nextIndex;

    if(isAdvancingTail){
        historyTail_ = (historyTail_ + 1) % maximumSize;
        historyStackSize_ = maximumSize;
    }else{
        if(historyHead_ >= historyTail_){
            historyStackSize_ = historyHead_ - historyTail_ + 1;
        }else{
            historyStackSize_ = (maximumSize - historyTail_) + historyHead_ + 1;
        }
    }

    DEBUG_PRINT("redo | cursorIndex_: {}/{}", cursorIndex_, historyStackSize_);
}

void ActionCenter::redo(){
    if(!canRedo()) return;
    
    cursorIndex_ = (cursorIndex_ + 1) % constants::action_center::MaximumHistory;
    
    stagedSlot_->data = commits_[cursorIndex_];

    DEBUG_PRINT("redo | cursorIndex_: {}/{}", cursorIndex_, historyStackSize_);
}

void ActionCenter::undo(){
    if(!canUndo()) return;

    if(cursorIndex_ == 0){
        cursorIndex_ = constants::action_center::MaximumHistory - 1;
    }else{
        cursorIndex_--;
    }

    stagedSlot_->data = commits_[cursorIndex_];

    DEBUG_PRINT("undo | cursorIndex_: {}/{}", cursorIndex_, historyStackSize_);
    
}

void ActionCenter::finishAction(){
    commit();
}