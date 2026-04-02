#include "navigation_bar.hpp"

#include <raygui.h>

#include "labels.hpp"

#include "interface/utilities.hpp"

#include "constants.hpp"

using namespace interface;
using namespace constants::labels::navigation_bar;

void NavigationBar::drawPerPageSettings(program_states::Context &context){
	const auto anchor{context.layout.anchor.navigationBar.perPageSettings};
	const auto &bounds{context.layout.bounds.navigationBar.perPageSettings};
	const int previousNotePerPageSpinnerValue{context.interface.navigationBar.notePerPageSpinnerValue};

	GuiGroupBox(calculateBoundsAtAnchor(anchor, bounds.groupBox), PerPageSettingsGroupBoxText);
	if(GuiSpinner(
		calculateBoundsAtAnchor(anchor, bounds.notePerPageSpinner),
		NotePerPageSpinnerText,
		&context.interface.navigationBar.notePerPageSpinnerValue,
		constants::project_data::MinimumNotePerPage,
		constants::project_data::MaximumNotePerPage,
		context.interface.navigationBar.notePerPageSpinnerEditMode
	)){
		context.interface.navigationBar.notePerPageSpinnerEditMode = !context.interface.navigationBar.notePerPageSpinnerEditMode;
	}

	if(context.interface.navigationBar.notePerPageSpinnerValue != previousNotePerPageSpinnerValue){
		context.interface.noteCanvas.isGridLayoutDirty = true;
	}
}
