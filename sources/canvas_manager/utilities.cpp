#include "canvas_manager.hpp"

#include "utilities/project_utilities.hpp"

#include "constants.hpp"

#include <raylib.h>

const program_states::Interface::NoteCanvas::CursorPosition &CanvasManager::cursorPosition() const{
    return context_.interface.noteCanvas.cursorPosition.value();
}

std::optional<int> CanvasManager::selectedInstrumentChannelIndex() const{
    return utilities::selectedInstrumentChannelIndex(context_.interface.sidebar.selectedChannelListViewIndex);
}

void CanvasManager::clearSelectionState(){
    context_.interface.clipboard.selectionArea = std::nullopt;
    isSelectionDragInProgress_ = false;
}