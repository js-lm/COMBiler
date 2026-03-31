#include "toolbar.hpp"

#include <raygui.h>

#include "layout.hpp"
#include "labels.hpp"

using namespace interface;
using namespace constants::layout::toolbar;
using namespace constants::labels::toolbar;

void Toolbar::drawSystemAction(SystemState &systemState){
	GuiGroupBox(SystemActionsGroupBox, SystemActionsGroupBoxText);

	systemState.toolbar.isMenuButtonPressed = GuiButton(MenuButton, MenuButtonText);
	systemState.toolbar.isNewFileButtonPressed = GuiButton(NewFileButton, NewFileButtonText);
	systemState.toolbar.isLoadFileButtonPressed = GuiButton(LoadFileButton, LoadFileButtonText);
	systemState.toolbar.isSaveFileButtonPressed = GuiButton(SaveFileButton, SaveFileButtonText);
	systemState.toolbar.isExportFileButtonPressed = GuiButton(ExportFileButton, ExportFileButtonText);

	systemState.toolbar.isMusicSettingButtonPressed = GuiButton(MusicSettingButton, MusicSettingButtonText);
    
}