#include "toolbar.hpp"

#include <raygui.h>

#include "labels.hpp"

#include "interface/utilities.hpp"

using namespace interface;
using namespace constants::labels::toolbar;

void Toolbar::drawToolPalette(program_states::Context &context){
	const auto anchor{context.layout.anchor.toolbar.toolPalette};
	const auto &bounds{context.layout.bounds.toolbar.toolPalette};

	GuiGroupBox(calculateBoundsAtAnchor(anchor, bounds.groupBox), ToolPaletteGroupBoxText);
	GuiToggleGroup(calculateBoundsAtAnchor(anchor, bounds.toolsToggleGroup), ToolsToggleGroupText, &context.interface.toolbar.selectedToolIndex);
	GuiLabel(calculateBoundsAtAnchor(anchor, bounds.toolsLabel), ToolsLabelText);

	context.interface.toolbar.isCutNoteButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.cutNoteButton), CutNoteButtonText);
	context.interface.toolbar.isCopyNoteButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.copyNoteButton), CopyNoteButtonText);
	context.interface.toolbar.isPasteNoteButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.pasteNoteButton), PasteNoteButtonText);

    
}