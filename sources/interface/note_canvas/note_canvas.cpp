#include "note_canvas.hpp"

#include <raygui.h>

#include "labels.hpp"

#include "interface/utilities.hpp"

using namespace interface;
using namespace constants::labels::note_canvas;

void NoteCanvas::draw(program_states::Context &context){
	const auto anchor{context.layout.anchor.noteCanvas.noteCanvas};
	const auto &bounds{context.layout.bounds.noteCanvas};

	const auto groupBox{calculateBoundsAtAnchor(anchor, bounds.groupBox)};

	GuiGroupBox(groupBox, NoteCanvasGroupBoxText);

}

