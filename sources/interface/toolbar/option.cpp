#include "toolbar.hpp"

#include <raygui.h>

#include "labels.hpp"

#include "interface/utilities.hpp"

#include "utilities/project_utilities.hpp"

using namespace interface;
namespace toolbar_constants = constants::labels::toolbar;

void Toolbar::drawOption(program_states::InterfaceContext &context){
	const auto anchor{context.layout.anchor.toolbar.toolOption};
	const auto &bounds{context.layout.bounds.toolbar.toolOption};

	const auto groupBox{calculateBoundsAtAnchor(anchor, bounds.groupBox)};

	GuiGroupBox(groupBox, toolbar_constants::ToolOptionsGroupBoxText);

	const auto projectData{utilities::projectDataFrom(context.system)};
	if(projectData && projectData->metadata.isReadOnly){
        if(context.interface.toolbar.readOnlyBlinkTimer > .0f){
            context.interface.toolbar.readOnlyBlinkTimer -= GetFrameTime();
            if(context.interface.toolbar.readOnlyBlinkTimer < .0f){
                context.interface.toolbar.readOnlyBlinkTimer = .0f;
            }
        }
        
        int oldColor{GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)};
        if(context.interface.toolbar.readOnlyBlinkTimer > .0f && static_cast<int>(context.interface.toolbar.readOnlyBlinkTimer * constants::toolbar::ReadOnlyBlinkFrequency) % 2 == 0){
            GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(RED));
        }

		GuiLabel(calculateBoundsAtAnchor(anchor, bounds.readOnlyIcon), toolbar_constants::ReadOnlyIconText);

        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, oldColor);
	}

	context.interface.toolbar.isInfoButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.infoButton), toolbar_constants::InfoButtonText);
	if(context.interface.toolbar.isInfoButtonPressed){
		context.interface.prompts.isInfoWindowVisible = true;
	}
}