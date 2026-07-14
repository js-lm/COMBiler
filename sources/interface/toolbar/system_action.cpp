#include "toolbar.hpp"

#include <raygui.h>

#include "labels.hpp"

#include "interface/utilities.hpp"

using namespace interface;
namespace toolbar_constants = constants::labels::toolbar;

void Toolbar::drawSystemAction(program_states::InterfaceContext &context){
	const auto anchor{context.layout.anchor.toolbar.systemAction};
	const auto &bounds{context.layout.bounds.toolbar.systemAction};

	GuiGroupBox(calculateBoundsAtAnchor(anchor, bounds.groupBox), toolbar_constants::SystemActionsGroupBoxText);

	context.interface.toolbar.isMenuButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.menuButton), toolbar_constants::MenuButtonText);
	if(context.interface.toolbar.isMenuButtonPressed){
		context.interface.prompts.isMenuWindowVisible = true;
	}
	context.interface.toolbar.isNewFileButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.newFileButton), toolbar_constants::NewFileButtonText);
	context.interface.toolbar.isLoadFileButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.loadFileButton), toolbar_constants::LoadFileButtonText);
	context.interface.toolbar.isSaveFileButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.saveFileButton), toolbar_constants::SaveFileButtonText);
	context.interface.toolbar.isSaveAsFileButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.saveAsFileButton), toolbar_constants::SaveAsFileButtonText);
	context.interface.toolbar.isExportFileButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.exportFileButton), toolbar_constants::ExportFileButtonText);

	context.interface.toolbar.isMusicSettingButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.musicSettingButton), toolbar_constants::MusicSettingButtonText);
    
}