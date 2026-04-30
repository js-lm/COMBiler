#include "toolbar.hpp"

#include <raygui.h>

#include "labels.hpp"

#include "interface/utilities.hpp"

using namespace interface;
namespace toolbar_constants = constants::labels::toolbar;

void Toolbar::drawStateControl(program_states::InterfaceContext &context){
	const auto anchor{context.layout.anchor.toolbar.stateControl};
	const auto &bounds{context.layout.bounds.toolbar.stateControl};

	GuiGroupBox(calculateBoundsAtAnchor(anchor, bounds.groupBox), toolbar_constants::StateControlGroupBoxText);
    
	context.interface.toolbar.isUndoButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.undoButton), toolbar_constants::UndoButtonText);
	context.interface.toolbar.isRedoButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.redoButton), toolbar_constants::RedoButtonText);
	context.interface.toolbar.isConstantsButtonPressed = GuiButton(
		calculateBoundsAtAnchor(anchor, bounds.constantsButton),
		toolbar_constants::ConstantsButtonText
	);

    
}