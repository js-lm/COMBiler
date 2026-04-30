#include "toolbar.hpp"

#include <raygui.h>

#include "labels.hpp"

#include "interface/utilities.hpp"

using namespace interface;
namespace toolbar_constants = constants::labels::toolbar;

void Toolbar::drawOption(program_states::InterfaceContext &context){
	const auto anchor{context.layout.anchor.toolbar.toolOption};
	const auto &bounds{context.layout.bounds.toolbar.toolOption};

	const auto groupBox{calculateBoundsAtAnchor(anchor, bounds.groupBox)};

	GuiGroupBox(groupBox, toolbar_constants::ToolOptionsGroupBoxText);
	context.interface.toolbar.isInfoButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.infoButton), toolbar_constants::InfoButtonText);

    
}