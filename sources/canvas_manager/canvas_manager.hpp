#pragma once

#include "program_states/context.hpp"

#include "action_center/action_center.hpp"

class CanvasManager{
private:
    program_states::InterfaceContext context_;

    // ActionCenter &actionCenter_;

    bool hasActionStarted_{false};
    bool isSelectionDragInProgress_{false};
    bool hasInitializedSelectionTracking_{false};

    int selectionDragStartColumnIndex_{0};
    int selectionDragStartRowIndex_{0};

    constants::toolbar::Tool previousSelectedTool_{constants::toolbar::Tool::Cursor};
    int previousSelectedChannelListViewIndex_{0};

public:

    CanvasManager(program_states::InterfaceContext context)
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

private:
    void handleSelectAll();
    void handleCopyAndPasteModeState();

private:
    void clearSelectionState();
    bool hasSelectionArea() const;
    bool isCommandChannelSelected() const;
};