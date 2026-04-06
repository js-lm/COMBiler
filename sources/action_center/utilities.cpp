#include "action_center.hpp"

#include "debug_utilities.hpp"

void ActionCenter::commit(){
    const size_t maximumSize{constants::action_center::MaximumHistory};
    const auto committedSnapshot{std::make_shared<program_states::ProjectData>(*stagedSlot_->data)};

    // if(historyStackSize_ == 0){
    //     commits_[0] = committedSnapshot;
    //     stagedSlot_->data = committedSnapshot;
    //     historyTail_ = 0;
    //     historyHead_ = 0;
    //     cursorIndex_ = 0;
    //     historyStackSize_ = 1;

    //     return;
    // }

    size_t nextIndex{(cursorIndex_ + 1) % maximumSize};
    
    bool wasFull{historyStackSize_ == maximumSize};
    bool isAdvancingTail{wasFull && (nextIndex == historyTail_ || cursorIndex_ == historyHead_)};

    commits_[nextIndex] = committedSnapshot;
    cursorIndex_ = nextIndex;
    historyHead_ = nextIndex;
    stagedSlot_->data = committedSnapshot;

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

    DEBUG_PRINT("commit | cursorIndex_: {}/{}", cursorIndex_, historyStackSize_);
}

bool ActionCenter::redo(){
    if(!canRedo() || isInAction_) return false;
    
    cursorIndex_ = (cursorIndex_ + 1) % constants::action_center::MaximumHistory;
    if(!commits_[cursorIndex_]) return false;
    
    stagedSlot_->data = commits_[cursorIndex_];

    DEBUG_PRINT("redo | cursorIndex_: {}/{}", cursorIndex_, historyStackSize_);

    return true;
}

bool ActionCenter::undo(){
    if(!canUndo() || isInAction_) return false;

    if(cursorIndex_ == 0){
        cursorIndex_ = constants::action_center::MaximumHistory - 1;
    }else{
        cursorIndex_--;
    }

    if(!commits_[cursorIndex_]) return false;

    stagedSlot_->data = commits_[cursorIndex_];

    DEBUG_PRINT("undo | cursorIndex_: {}/{}", cursorIndex_, historyStackSize_);
    
    return true;
    
}

void ActionCenter::beginAction(){
    if(isInAction_) return;

    stagedSlot_->data = std::make_shared<program_states::ProjectData>(*stagedSlot_->data);
    isInAction_ = true;
}

void ActionCenter::finishAction(){
    if(!isInAction_) return;

    commit();
    isInAction_ = false;
}

const program_states::ProjectData::Transient *ActionCenter::currentTransient() const{
    return &stagedSlot_->data->transient;
}