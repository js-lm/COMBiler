#include "toolbar.hpp"

#include <raygui.h>

#include "layout.hpp"
#include "labels.hpp"

using namespace interface;
using namespace constants::layout::toolbar;
using namespace constants::labels::toolbar;

void Toolbar::drawStateControl(SystemState &systemState){

	GuiGroupBox(StateControlGroupBox, StateControlGroupBoxText);
    
	systemState.toolbar.isUndoButtonPressed = GuiButton(UndoButton, UndoButtonText);
	systemState.toolbar.isRedoButtonPressed = GuiButton(RedoButton, RedoButtonText);

	if(GuiDropdownBox(
        ActionHistoryDropdownBox,
        ActionHistoryDropdownBoxText,
        &systemState.toolbar.actionHistoryDropdownActive,
        systemState.toolbar.actionHistoryDropdownEditMode
    )){
		systemState.toolbar.actionHistoryDropdownEditMode = !systemState.toolbar.actionHistoryDropdownEditMode;
	}

    
}