#include "navigation_bar.hpp"

#include <raygui.h>

#include "labels.hpp"

#include "interface/utilities.hpp"

using namespace interface;
using namespace constants::labels::navigation_bar;

void NavigationBar::drawPerPageSettings(program_states::Context &context){
	const auto anchor{context.layout.anchor.navigationBar.perPageSettings};
	const auto &bounds{context.layout.bounds.navigationBar.perPageSettings};

	GuiGroupBox(calculateBoundsAtAnchor(anchor, bounds.groupBox), PerPageSettingsGroupBoxText);
	if(GuiSpinner(
		calculateBoundsAtAnchor(anchor, bounds.notePerPageSpinner),
		NotePerPageSpinnerText,
		&context.interface.navigationBar.notePerPageSpinnerValue,
		0,
		100, // TODO: magic numbers
		context.interface.navigationBar.notePerPageSpinnerEditMode
	)){
		context.interface.navigationBar.notePerPageSpinnerEditMode = !context.interface.navigationBar.notePerPageSpinnerEditMode;
	}
}
