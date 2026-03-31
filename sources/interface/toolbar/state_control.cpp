#include "toolbar.hpp"

#include <raygui.h>

#include "labels.hpp"

#include "interface/utilities.hpp"

using namespace interface;
using namespace constants::labels::toolbar;

void Toolbar::drawStateControl(program_states::Context &context){
	const auto anchor{context.layout.anchor.toolbar.stateControl};
	const auto &bounds{context.layout.bounds.toolbar.stateControl};

	GuiGroupBox(calculateBoundsAtAnchor(anchor, bounds.groupBox), StateControlGroupBoxText);
    
	context.interface.toolbar.isUndoButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.undoButton), UndoButtonText);
	context.interface.toolbar.isRedoButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.redoButton), RedoButtonText);

	if(GuiDropdownBox(
        calculateBoundsAtAnchor(anchor, bounds.actionHistoryDropdownBox),
        ActionHistoryDropdownBoxText,
		&context.interface.toolbar.actionHistoryDropdownActive,
		context.interface.toolbar.actionHistoryDropdownEditMode
    )){
		context.interface.toolbar.actionHistoryDropdownEditMode = !context.interface.toolbar.actionHistoryDropdownEditMode;
	}

    
}