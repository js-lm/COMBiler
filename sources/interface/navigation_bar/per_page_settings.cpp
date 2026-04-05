#include "navigation_bar.hpp"

#include <raygui.h>

#include "labels.hpp"

#include "interface/utilities.hpp"

#include "constants.hpp"

#include <algorithm>

using namespace interface;
using namespace constants::labels::navigation_bar;

void NavigationBar::drawPerPageSettings(program_states::InterfaceContext &context){
	const auto anchor{context.layout.anchor.navigationBar.perPageSettings};
	const auto &bounds{context.layout.bounds.navigationBar.perPageSettings};

	const auto projectDataSlot{context.system.project.data.lock()};
	if(projectDataSlot && !projectDataSlot->data->pages.empty()){
		const auto &projectData{projectDataSlot->data};

		const int currentPageIndex{
			std::clamp(
				context.system.project.currentPage - 1,
				0,
				static_cast<int>(projectData->pages.size()) - 1
			)
		};

		const int currentPageNoteCount{projectData->pages[currentPageIndex].noteInThisPage.value_or(projectData->metadata.noteInThisPage)};

		if(context.interface.navigationBar.notePerPageSpinnerValue != currentPageNoteCount){
			context.interface.navigationBar.notePerPageSpinnerValue = currentPageNoteCount;
			context.interface.noteCanvas.isGridLayoutDirty = true;
		}
	}

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
		if(projectDataSlot && !projectDataSlot->data->pages.empty()){
			const auto &projectData{projectDataSlot->data};

			const int currentPageIndex{
				std::clamp(
					context.system.project.currentPage - 1,
					0,
					static_cast<int>(projectData->pages.size()) - 1
				)
			};

			const int updatedNoteCount{
				std::clamp(
					context.interface.navigationBar.notePerPageSpinnerValue,
					constants::project_data::MinimumNotePerPage,
					constants::project_data::MaximumNotePerPage
				)
			};

			if(updatedNoteCount == projectData->metadata.noteInThisPage){
				projectData->pages[currentPageIndex].noteInThisPage = std::nullopt;
			}else{
				projectData->pages[currentPageIndex].noteInThisPage = updatedNoteCount;
			}
		}

		context.interface.noteCanvas.isGridLayoutDirty = true;
	}
}
