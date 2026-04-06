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
    size_t historyStackSize_{1};

    std::shared_ptr<StagedSlot> stagedSlot_{std::make_shared<StagedSlot>(
        std::make_shared<program_states::ProjectData>(program_states::DEBUG_preset::projectData)
    )};

    bool isInAction_{false};

public:
    ActionCenter(){
        commits_.resize(constants::action_center::MaximumHistory);
        // commit();
        const auto committedSnapshot{std::make_shared<program_states::ProjectData>(*stagedSlot_->data)};

        commits_[0] = committedSnapshot;
        stagedSlot_->data = committedSnapshot;

    }
    ~ActionCenter() = default;

public:
    std::weak_ptr<StagedSlot> getStagedObserver() const{ return stagedSlot_;}

    const program_states::ProjectData::Transient *currentTransient() const;

public:
    bool redo();
    bool undo();
    bool canRedo() const{ return historyStackSize_ > 1 && cursorIndex_ != historyHead_;}
    bool canUndo() const{ return historyStackSize_ > 1 && cursorIndex_ != historyTail_;}

    void addNote(int pageNumber, int channelIndex, int noteIndex, music_data::Note note);
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
    void beginAction();
    void commit();
};