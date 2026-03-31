#include "sidebar.hpp"

#include <raygui.h>

#include "layout.hpp"
#include "labels.hpp"

using namespace interface;
using namespace constants::layout::sidebar;
using namespace constants::labels::sidebar;

void Sidebar::drawInspector(SystemState &systemState){
	GuiGroupBox(InspectorGroupBox, InspectorGroupBoxText);
	GuiListView(
		ChannelListView,
		ChannelListViewText,
		&systemState.sidebar.selectedChannelListViewScrollIndex,
		&systemState.sidebar.selectedChannelListViewIndex
	);
	GuiLabel(ChannelLabel, ChannelLabelText);
	GuiLine(StructureLine, StructureLineText);

	if(GuiValueBox(
		TempoValueBox,
		TempoValueBoxText,
		&systemState.sidebar.tempoValue,
		0,
		100, // TODO: magic numbers
		systemState.sidebar.tempoValueBoxEditMode
	)){
		systemState.sidebar.tempoValueBoxEditMode = !systemState.sidebar.tempoValueBoxEditMode;
	}

	if(GuiValueBox(
		NotesValueBox,
		NotesValueBoxText,
		&systemState.sidebar.notesValue,
		0,
		100, // TODO: magic numbers
		systemState.sidebar.notesValueBoxEditMode
	)){
		systemState.sidebar.notesValueBoxEditMode = !systemState.sidebar.notesValueBoxEditMode;
	}

	if(GuiValueBox(
		BarsValueBox,
		BarsValueBoxText,
		&systemState.sidebar.barsValue,
		0,
		100, // TODO: magic numbers
		systemState.sidebar.barsValueBoxEditMode
	)){
		systemState.sidebar.barsValueBoxEditMode = !systemState.sidebar.barsValueBoxEditMode;
	}
}
