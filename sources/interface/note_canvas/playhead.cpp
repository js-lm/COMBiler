#include "note_canvas.hpp"

#include <raygui.h>

#include "constants.hpp"
#include "debug_utilities.hpp"

using namespace interface;

namespace PlayheadConstants = constants::interface_layout::note_canvas::playhead;

void NoteCanvas::drawPlayhead(program_states::InterfaceContext &context){

    if(!context.machine.isPlaying) return;

    const auto anchor{context.layout.anchor.noteCanvas.noteCanvas};
	const auto &bounds{context.layout.bounds.noteCanvas};
    const auto &state{context.interface.noteCanvas};

    // context.machine.playheadIndex;
    const float positionX{
        anchor.x + state.gridArea.x + (static_cast<float>(context.machine.playheadIndex) * state.columnWidth)
    };
    const float positionY{anchor.y};

    const Rectangle playheadBounds{
        positionX,
        positionY,
        state.columnWidth,
        bounds.groupBox.height
    };

    DrawRectangleRec(playheadBounds, Fade(RED, PlayheadConstants::BackgroundAlpha));
    DrawLineEx(
        Vector2{positionX + state.columnWidth * .5f, positionY},
        Vector2{positionX + state.columnWidth * .5f, positionY + bounds.groupBox.height},
        PlayheadConstants::HeadThickness,
        RED
    );
}