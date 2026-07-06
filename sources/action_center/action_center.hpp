#pragma once

#include <vector>
#include <memory>
#include <functional>

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
        std::make_shared<program_states::ProjectData>(program_states::ProjectData{})
    )};

    bool isInAction_{false};

    std::optional<size_t> savedCursorIndex_{0};

public:
    std::function<void()> onReadOnlyViolation{};

public:
    void markAsSaved(){ savedCursorIndex_ = cursorIndex_;}
    void markAsUnsaved(){ savedCursorIndex_ = std::nullopt;}
    bool isUnsaved() const{ return !savedCursorIndex_.has_value() || savedCursorIndex_.value() != cursorIndex_;}

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
    void removePage(int startIndex, int endIndex);
    void copyPage(program_states::Interface &interface, int startIndex, int endIndex);
    void pastePage(program_states::Interface &interface, int insertionIndex);
    void cutPage(program_states::Interface &interface, int startIndex, int endIndex);

    void addCommand(int pageNumber, int noteIndex, const program_states::Interface::Prompts &promptState);
    void removeCommand(int pageNumber, int noteIndex);
    void setCommandToken(
        int pageNumber,
        int noteIndex,
        int selectedChannelListViewIndex,
        std::optional<command::CommandToken> commandToken
    );

    void commitConstantModification(const program_states::Interface::Prompts::ConstantsManagerDraft &draft);

    void updateInstrumentChannelCell(
        int pageNumber,
        int channelIndex,
        int noteIndex,
        std::optional<music_data::InstrumentChannelData> cellValue
    );

public:
    void loadFile(program_states::ProjectData &projectData);

public:
    void finishAction();

private:
    bool beginAction();
    void commit();

private:
    command::Target targetFromToggleIndex(int toggleIndex);
    units::machine::Articulation articulationFromToggleIndex(int toggleIndex);
    command::Command commandFromPromptState(const program_states::Interface::Prompts &promptState);

    command::Command constantFromPromptState(const program_states::Interface::Prompts &promptState);
};