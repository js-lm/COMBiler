#include "toolbar.hpp"

#include <raygui.h>

#include "labels.hpp"

#include "interface/utilities.hpp"

using namespace interface;
using namespace constants::labels::toolbar;

void Toolbar::drawSystemAction(program_states::InterfaceContext &context){
	const auto anchor{context.layout.anchor.toolbar.systemAction};
	const auto &bounds{context.layout.bounds.toolbar.systemAction};

	GuiGroupBox(calculateBoundsAtAnchor(anchor, bounds.groupBox), SystemActionsGroupBoxText);

	context.interface.toolbar.isMenuButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.menuButton), MenuButtonText);
	context.interface.toolbar.isNewFileButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.newFileButton), NewFileButtonText);
	context.interface.toolbar.isLoadFileButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.loadFileButton), LoadFileButtonText);
	context.interface.toolbar.isSaveFileButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.saveFileButton), SaveFileButtonText);
	context.interface.toolbar.isSaveAsFileButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.saveAsFileButton), SaveAsFileButtonText);
	context.interface.toolbar.isExportFileButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.exportFileButton), ExportFileButtonText);

	context.interface.toolbar.isMusicSettingButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.musicSettingButton), MusicSettingButtonText);
    
}