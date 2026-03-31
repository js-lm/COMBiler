#include "navigation_bar.hpp"

#include <raygui.h>

#include "layout.hpp"
#include "labels.hpp"

using namespace interface;
using namespace constants::layout::navigation_bar;
using namespace constants::labels::navigation_bar;

void NavigationBar::drawPerPageSettings(SystemState &systemState){
	GuiGroupBox(PerPageSettingsGroupBox, PerPageSettingsGroupBoxText);
	if(GuiSpinner(
		NotePerPageSpinner,
		NotePerPageSpinnerText,
		&systemState.navigationBar.notePerPageSpinnerValue,
		0,
		100, // TODO: magic numbers
		systemState.navigationBar.notePerPageSpinnerEditMode
	)){
		systemState.navigationBar.notePerPageSpinnerEditMode = !systemState.navigationBar.notePerPageSpinnerEditMode;
	}
}
