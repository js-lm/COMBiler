#pragma once

#include <vector>
#include <memory>

#include "program_states/project_data.hpp"

#include "constants.hpp"

class ActionCenter{
public:
    struct StagedSlot{ std::shared_ptr<program_states::ProjectData> data;};

private:
    std::vector<std::shared_ptr<program_states::ProjectData>> commits_;

    size_t historyTail_{0};
    size_t historyHead_{0};
    size_t cursorIndex_{0};
    size_t historyStackSize_{0};

    std::shared_ptr<StagedSlot> stagedSlot_{std::make_shared<StagedSlot>(
        std::make_shared<program_states::ProjectData>(program_states::DEBUG_preset::projectData)
    )};

    bool isInAction_{false};

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

    void addNote(int pageNumber, int noteIndex, music_data::Note note);
    void removeNote(int pageNumber, int noteIndex);
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