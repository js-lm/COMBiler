#include "toolbar.hpp"

#include <raygui.h>

#include "labels.hpp"

#include "interface/utilities.hpp"

using namespace interface;
using namespace constants::labels::toolbar;

void Toolbar::drawOption(program_states::Context &context){
	const auto anchor{context.layout.anchor.toolbar.toolOption};
	const auto &bounds{context.layout.bounds.toolbar.toolOption};

	const auto groupBox{calculateBoundsAtAnchor(anchor, bounds.groupBox)};

	GuiGroupBox(groupBox, ToolOptionsGroupBoxText);

    
}