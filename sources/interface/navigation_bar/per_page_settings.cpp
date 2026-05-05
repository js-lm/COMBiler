#include "navigation_bar.hpp"

#include <raygui.h>

#include "labels.hpp"

#include "interface/utilities.hpp"

#include "constants.hpp"

#include <algorithm>

#include "utilities/project_utilities.hpp"

using namespace interface;
namespace bar_labels = constants::labels::navigation_bar;

void NavigationBar::drawPerPageSettings(program_states::InterfaceContext &context){
	const auto anchor{context.layout.anchor.navigationBar.perPageSettings};
	const auto &bounds{context.layout.bounds.navigationBar.perPageSettings};

	const auto projectData{utilities::projectDataWithPagesFrom(context.system)};
	if(projectData){
		const int currentPageNoteCount{utilities::currentPageNoteCountFrom(*projectData, context.system.project.currentPage)};

		if(context.interface.navigationBar.notePerPageSpinnerValue != currentPageNoteCount){
			context.interface.navigationBar.notePerPageSpinnerValue = currentPageNoteCount;
			context.interface.noteCanvas.isGridLayoutDirty = true;
		}
	}

	const int previousNotePerPageSpinnerValue{context.interface.navigationBar.notePerPageSpinnerValue};

	const bool isPlaying{context.machine.isPlaying};
	if(isPlaying) GuiDisable();

	GuiGroupBox(calculateBoundsAtAnchor(anchor, bounds.groupBox), bar_labels::PerPageSettingsGroupBoxText);
	if(GuiSpinner(
		calculateBoundsAtAnchor(anchor, bounds.notePerPageSpinner),
		bar_labels::NotePerPageSpinnerText,
		&context.interface.navigationBar.notePerPageSpinnerValue,
		constants::project_data::MinimumNotePerPage,
		constants::project_data::MaximumNotePerPage,
		context.interface.navigationBar.notePerPageSpinnerEditMode
	)){
		context.interface.navigationBar.notePerPageSpinnerEditMode = !context.interface.navigationBar.notePerPageSpinnerEditMode;
	}

	if(isPlaying) GuiEnable();

	if(context.interface.navigationBar.notePerPageSpinnerValue != previousNotePerPageSpinnerValue){
		if(projectData){
			const int currentPageIndex{utilities::currentPageIndexFrom(*projectData, context.system.project.currentPage)};

			const int updatedNoteCount{
				std::clamp(
					context.interface.navigationBar.notePerPageSpinnerValue,
					constants::project_data::MinimumNotePerPage,
					constants::project_data::MaximumNotePerPage
				)
			};

			if(updatedNoteCount == projectData->metadata.notePerPage){
				projectData->pages[currentPageIndex].noteInThisPage = std::nullopt;
			}else{
				projectData->pages[currentPageIndex].noteInThisPage = updatedNoteCount;
			}
		}

		context.interface.noteCanvas.isGridLayoutDirty = true;
	}
}
