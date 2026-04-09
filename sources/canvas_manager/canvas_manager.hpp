#pragma once

#include "program_states/context.hpp"

#include "action_center/action_center.hpp"

class CanvasManager{
private:
    program_states::ReadOnlyContext context_;

    // ActionCenter &actionCenter_;

    bool hasActionStarted_{false};

public:

    CanvasManager(program_states::ReadOnlyContext context)
        : context_{context}
    {}
    CanvasManager() = delete;
    ~CanvasManager() = default;

    void update(ActionCenter &actionCenter);

private:
    void handleNoteTools(ActionCenter &actionCenter);

    void handleSelection(ActionCenter &actionCenter);
    void handleNoteAdding(ActionCenter &actionCenter);
    void handleNoteDeletion(ActionCenter &actionCenter);
    void handleInstrumentChange(ActionCenter &actionCenter);

private:
    bool handleCommandTools(ActionCenter &actionCenter);

private:
    void handlePageTools(ActionCenter &actionCenter);

private:
    const program_states::Interface::NoteCanvas::CursorPosition &cursorPosition() const;
    std::optional<int> selectedInstrumentChannelIndex() const;
};