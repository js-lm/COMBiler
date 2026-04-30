#include "note_canvas.hpp"

#include <raygui.h>

#include "labels.hpp"

#include "interface/utilities.hpp"

#include "debug_utilities.hpp"

using namespace interface;
namespace note_canvas_labels = constants::labels::note_canvas;

void NoteCanvas::draw(program_states::InterfaceContext &context){
	const auto anchor{context.layout.anchor.noteCanvas.noteCanvas};
	const auto &bounds{context.layout.bounds.noteCanvas};

	const auto groupBox{calculateBoundsAtAnchor(anchor, bounds.groupBox)};
	auto &state{context.interface.noteCanvas};

	if(state.isGridLayoutDirty) cleanGridLayout(context);

	// DEBUG_PRINT_IF_CHANGED(
	// 	"dirtyLayout: {}",
	// 	// groupBox.x,
	// 	// groupBox.y,
	// 	// groupBox.width,
	// 	// groupBox.height,
	// 	context.interface.noteCanvas.isGridLayoutDirty
	// );

	GuiGroupBox(groupBox, note_canvas_labels::NoteCanvasGroupBoxText);
	handleZoom(context);

	drawGrid(context);
	drawScrollBar(context);
	drawPlayhead(context);
}