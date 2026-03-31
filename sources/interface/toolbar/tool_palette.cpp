#include "toolbar.hpp"

#include <raygui.h>

#include "layout.hpp"
#include "labels.hpp"

using namespace interface;
using namespace constants::layout::toolbar;
using namespace constants::labels::toolbar;

void Toolbar::drawToolPalette(SystemState &systemState){
	GuiGroupBox(ToolPaletteGroupBox, ToolPaletteGroupBoxText);
	GuiToggleGroup(ToolsToggleGroup, ToolsToggleGroupText, &systemState.toolbar.selectedToolIndex);
	GuiLabel(ToolsLabel, ToolsLabelText);

	systemState.toolbar.isCutNoteButtonPressed = GuiButton(CutNoteButton, CutNoteButtonText);
	systemState.toolbar.isCopyNoteButtonPressed = GuiButton(CopyNoteButton, CopyNoteButtonText);
	systemState.toolbar.isPasteNoteButtonPressed = GuiButton(PasteNoteButton, PasteNoteButtonText);

    
}