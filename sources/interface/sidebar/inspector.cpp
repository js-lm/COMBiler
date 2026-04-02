#include "sidebar.hpp"

#include <raygui.h>

#include "labels.hpp"

#include "interface/utilities.hpp"

#include <cmath>

using namespace interface;
using namespace constants::labels::sidebar;

void Sidebar::drawInspector(program_states::Context &context){
	const auto anchor{context.layout.anchor.sidebar.inspect};
	const auto &bounds{context.layout.bounds.sidebar.inspector};

	GuiGroupBox(calculateBoundsAtAnchor(anchor, bounds.groupBox), InspectorGroupBoxText);
	GuiListView(
		calculateBoundsAtAnchor(anchor, bounds.channelListView),
		ChannelListViewText,
		&context.interface.sidebar.selectedChannelListViewScrollIndex,
		&context.interface.sidebar.selectedChannelListViewIndex
	);
	context.interface.sidebar.selectedChannelListViewIndex = std::max(0, context.interface.sidebar.selectedChannelListViewIndex);
	GuiLabel(calculateBoundsAtAnchor(anchor, bounds.channelLabel), ChannelLabelText);
	GuiLine(calculateBoundsAtAnchor(anchor, bounds.structureLine), StructureLineText);

	if(GuiValueBox(
		calculateBoundsAtAnchor(anchor, bounds.tempoValueBox),
		TempoValueBoxText,
		&context.interface.sidebar.tempoValue,
		0,
		100, // TODO: magic numbers
		context.interface.sidebar.tempoValueBoxEditMode
	)){
		context.interface.sidebar.tempoValueBoxEditMode = !context.interface.sidebar.tempoValueBoxEditMode;
	}

	if(GuiValueBox(
		calculateBoundsAtAnchor(anchor, bounds.notesValueBox),
		NotesValueBoxText,
		&context.interface.sidebar.notesValue,
		0,
		100, // TODO: magic numbers
		context.interface.sidebar.notesValueBoxEditMode
	)){
		context.interface.sidebar.notesValueBoxEditMode = !context.interface.sidebar.notesValueBoxEditMode;
	}

	if(GuiValueBox(
		calculateBoundsAtAnchor(anchor, bounds.barsValueBox),
		BarsValueBoxText,
		&context.interface.sidebar.barsValue,
		0,
		100, // TODO: magic numbers
		context.interface.sidebar.barsValueBoxEditMode
	)){
		context.interface.sidebar.barsValueBoxEditMode = !context.interface.sidebar.barsValueBoxEditMode;
	}
}
