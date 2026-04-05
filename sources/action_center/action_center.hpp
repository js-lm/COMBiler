#pragma once

#include <vector>
#include <memory>

#include "program_states/project_data.hpp"

#include "constants/action_center.hpp"

class ActionCenter{
private:
    std::vector<std::shared_ptr<program_states::ProjectData>> commits_;

    size_t historyTail_{0};
    size_t historyHead_{0};
    size_t cursorIndex_{0};
    size_t historyStackSize_{0};

    struct StagedSlot{ std::shared_ptr<program_states::ProjectData> data;};

    std::shared_ptr<StagedSlot> stagedSlot_;

public:
    ActionCenter(){
        commits_.resize(constants::action_center::MaximumHistory);
    }
    ~ActionCenter() = default;

public:
    std::weak_ptr<StagedSlot> getStagedObserver() const{ return stagedSlot_;}


public:
    void redo();
    void undo();
    bool canRedo() const{ return historyStackSize_ > 1 && cursorIndex_ != historyHead_;}
    bool canUndo() const{ return historyStackSize_ > 1 && cursorIndex_ != historyTail_;}

    void addNote();
    void removeNote();
    void copyNote();
    void pasteNote();

    void addPage();
    void removePage();
    void copyPage();
    void pastePage();  

public:
    void finishAction();

private:
    void commit();




};