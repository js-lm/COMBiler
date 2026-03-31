#include "note_canvas.hpp"

#include <raygui.h>

#include "layout.hpp"
#include "labels.hpp"

using namespace interface;
using namespace constants::layout::note_canvas;
using namespace constants::labels::note_canvas;

void NoteCanvas::draw(SystemState &systemState){
	GuiGroupBox(NoteCanvasGroupBox, NoteCanvasGroupBoxText);

}

