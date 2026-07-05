#include "sidebar.hpp"

#include <raygui.h>

#include "labels.hpp"

#include "interface/utilities.hpp"
#include "utilities/project_utilities.hpp"

#include "constants.hpp"

#include <cmath>

using namespace interface;
namespace sidebar_constants = constants::labels::sidebar;

void Sidebar::drawInspector(program_states::InterfaceContext &context){
	const auto anchor{context.layout.anchor.sidebar.inspect};
	const auto &bounds{context.layout.bounds.sidebar.inspector};
	const auto projectData{utilities::projectDataFrom(context.system)};

	if(projectData){
		const int clampedDefaultNoteCount{
			std::clamp(
				projectData->metadata.notePerPage,
				constants::project_data::MinimumNotePerPage,
				constants::project_data::MaximumNotePerPage
			)
		};

		if(context.interface.sidebar.notesValue != clampedDefaultNoteCount){
			context.interface.sidebar.notesValue = clampedDefaultNoteCount;
		}
	}

	const int previousNotesValue{context.interface.sidebar.notesValue};

	GuiGroupBox(calculateBoundsAtAnchor(anchor, bounds.groupBox), sidebar_constants::InspectorGroupBoxText);
	const int previousChannelIndex{context.interface.sidebar.selectedChannelListViewIndex};

	GuiListView(
		calculateBoundsAtAnchor(anchor, bounds.channelListView),
		sidebar_constants::ChannelListViewText,
		&context.interface.sidebar.selectedChannelListViewScrollIndex,
		&context.interface.sidebar.selectedChannelListViewIndex
	);
	context.interface.sidebar.selectedChannelListViewIndex = std::max(0, context.interface.sidebar.selectedChannelListViewIndex);

	// TODO: duplicated
	if(previousChannelIndex != context.interface.sidebar.selectedChannelListViewIndex){
        if(context.interface.prompts.activeCommandPrompt == constants::prompts::CommandPrompt::Constant){
            context.interface.prompts.activeCommandPrompt = constants::prompts::CommandPrompt::Tempo;
            context.interface.prompts.selectedCommandTool = constants::prompts::CommandPrompt::Tempo;
        }

        if(context.interface.sidebar.selectedChannelListViewIndex == constants::sidebar::AllChannelsListViewIndex){
            context.interface.toolbar.savedToolBeforeAllChannel = context.interface.toolbar.selectedTool;
            context.interface.toolbar.selectedTool = constants::toolbar::Tool::Cursor;
        }else if(previousChannelIndex == constants::sidebar::AllChannelsListViewIndex){
            context.interface.toolbar.selectedTool = context.interface.toolbar.savedToolBeforeAllChannel;
        }
	}

	GuiLabel(calculateBoundsAtAnchor(anchor, bounds.channelLabel), sidebar_constants::ChannelLabelText);
	// GuiLine(calculateBoundsAtAnchor(anchor, bounds.structureLine), sidebar_constants::StructureLineText);

	// if(GuiValueBox(
	// 	calculateBoundsAtAnchor(anchor, bounds.tempoValueBox),
	// 	sidebar_constants::TempoValueBoxText,
	// 	&context.interface.sidebar.tempoValue,
	// 	0,
	// 	100, // TODO: magic numbers
	// 	context.interface.sidebar.tempoValueBoxEditMode
	// )){
	// 	context.interface.sidebar.tempoValueBoxEditMode = !context.interface.sidebar.tempoValueBoxEditMode;
	// }

	// if(GuiValueBox(
	// 	calculateBoundsAtAnchor(anchor, bounds.notesValueBox),
	// 	sidebar_constants::NotesValueBoxText,
	// 	&context.interface.sidebar.notesValue,
	// 	constants::project_data::MinimumNotePerPage,
	// 	constants::project_data::MaximumNotePerPage,
	// 	context.interface.sidebar.notesValueBoxEditMode
	// )){
	// 	context.interface.sidebar.notesValueBoxEditMode = !context.interface.sidebar.notesValueBoxEditMode;
	// }

	// if(projectData && context.interface.sidebar.notesValue != previousNotesValue){
	// 	context.interface.sidebar.notesValue = std::clamp(
	// 		context.interface.sidebar.notesValue,
	// 		constants::project_data::MinimumNotePerPage,
	// 		constants::project_data::MaximumNotePerPage
	// 	);

	// 	projectData->metadata.notePerPage = context.interface.sidebar.notesValue;
	// 	const int currentPageNoteCount{utilities::currentPageNoteCountFrom(*projectData, context.system.project.currentPage)};
	// 	context.interface.navigationBar.notePerPageSpinnerValue = currentPageNoteCount;
	// 	context.interface.noteCanvas.isGridLayoutDirty = true;
	// }

	GuiLine(calculateBoundsAtAnchor(anchor, bounds.sideSettingLine), sidebar_constants::SideSettingLineText);
	GuiCheckBox(
		calculateBoundsAtAnchor(anchor, bounds.showCommandsCheckBox),
		sidebar_constants::ShowCommandsCheckBoxText,
		&context.interface.sidebar.isShowCommandsEnabled
	);
}
