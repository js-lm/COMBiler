#pragma once

#include <vector>
#include <memory>

#include "program_states/project_data.hpp"
#include "program_states/interface.hpp"

#include "constants.hpp"

#include "command/command_type.hpp"

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
        // const auto committedSnapshot{std::make_shared<program_states::ProjectData>(*stagedSlot_->data)};

        // commits_[0] = committedSnapshot;
        // stagedSlot_->data = committedSnapshot;

        commits_[0] = std::make_shared<program_states::ProjectData>(*stagedSlot_->data);

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
    void removeNote(int pageNumber, int channelIndex, int noteIndex);
    void addInstrumentChange(int pageNumber, int channelIndex, int noteIndex, music_data::Instrument instrument);

    void copy();
    void paste();

    void addPage(int requestedPageInsertionIndex);
    void movePage(int sourcePageIndex, int targetInsertionIndex);
    void removePage();
    void copyPage();
    void pastePage();  

    void addCommand(int pageNumber, int noteIndex, const program_states::Interface::Prompts &promptState);
    void removeCommand(int pageNumber, int noteIndex);
    void setCommandToken(
        int pageNumber,
        int noteIndex,
        int selectedChannelListViewIndex,
        std::optional<command::CommandToken> commandToken
    );

public:
    void finishAction();

private:
    void beginAction();
    void commit();

private:
    command::Target targetFromToggleIndex(int toggleIndex);
    units::machine::Articulation articulationFromToggleIndex(int toggleIndex);
    command::Command commandFromPromptState(const program_states::Interface::Prompts &promptState);

    void updateInstrumentChannelCell(
        int pageNumber,
        int channelIndex,
        int noteIndex,
        std::optional<music_data::InstrumentChannelData> cellValue
    );
};