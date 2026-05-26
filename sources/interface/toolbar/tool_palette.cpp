#include "toolbar.hpp"

#include <raygui.h>

#include "labels.hpp"

#include "interface/utilities.hpp"

#include "constants.hpp"

#include <algorithm>

#include "debug_utilities.hpp"

using namespace interface;
namespace toolbar_constants = constants::labels::toolbar;

void Toolbar::drawToolPalette(program_states::InterfaceContext &context){
	const auto anchor{context.layout.anchor.toolbar.toolPalette};
	const auto &bounds{context.layout.bounds.toolbar.toolPalette};

	auto &toolbarState{context.interface.toolbar};
	auto &promptState{context.interface.prompts};

	const bool isSystemChannelSelected{
		context.interface.sidebar.selectedChannelListViewIndex == constants::interface_layout::note_canvas::notes::SystemChannelListViewIndex
	};
	const bool isAllChannelSelected{
		context.interface.sidebar.selectedChannelListViewIndex == constants::interface_layout::note_canvas::notes::AllChannelsListViewIndex
	};

	GuiGroupBox(calculateBoundsAtAnchor(anchor, bounds.groupBox), toolbar_constants::ToolPaletteGroupBoxText);

	if(promptState.activeCommandPrompt != constants::prompts::CommandPrompt::Constant){
		GuiLabel(calculateBoundsAtAnchor(anchor, bounds.toolsLabel), toolbar_constants::ToolsLabelText);

	}

	if(isSystemChannelSelected){

		if(promptState.activeCommandPrompt == constants::prompts::CommandPrompt::Constant){
			if(GuiButton(calculateBoundsAtAnchor(anchor, bounds.constantExitButton), toolbar_constants::ExitConstantModeButtonText)){
				promptState.activeCommandPrompt = constants::prompts::CommandPrompt::Tempo;
				promptState.selectedCommandTool = constants::prompts::CommandPrompt::Tempo;
			}
		}else{
			int selectedCommandToolIndex{constants::prompts::toIndex(promptState.selectedCommandTool)};
			selectedCommandToolIndex = std::clamp(selectedCommandToolIndex, 0, constants::prompts::CommandToolCount - 2);
			promptState.selectedCommandTool = constants::prompts::commandPromptFromIndex(selectedCommandToolIndex);

			GuiToggleGroup(calculateBoundsAtAnchor(anchor, bounds.toolsToggleGroup), toolbar_constants::ToolsToggleGroupCommandText, &selectedCommandToolIndex);
			promptState.selectedCommandTool = constants::prompts::commandPromptFromIndex(selectedCommandToolIndex);
			promptState.activeCommandPrompt = promptState.selectedCommandTool;

			const Rectangle openCommandWindowButtonBounds{calculateBoundsAtAnchor(anchor, bounds.openCommandWindowButton)};

			if(GuiButton(openCommandWindowButtonBounds, toolbar_constants::OpenCommandWindowButtonText)){
				promptState.activeCommandPrompt = promptState.selectedCommandTool;
				promptState.isCommandWindowVisible = true;

			}
		}


	}else{
		if(isAllChannelSelected) toolbarState.selectedTool = constants::toolbar::Tool::Cursor; // TODO: restore the tool after switching back to other channel

		int selectedToolIndex{constants::toolbar::toIndex(toolbarState.selectedTool)};

		selectedToolIndex = std::clamp(selectedToolIndex, 0, constants::toolbar::ToolCount - 1);
		GuiToggleGroup(calculateBoundsAtAnchor(anchor, bounds.toolsToggleGroup), toolbar_constants::ToolsToggleGroupNormalText, &selectedToolIndex);

		toolbarState.selectedTool = constants::toolbar::toolFromIndex(selectedToolIndex);
		if(promptState.activeCommandPrompt != constants::prompts::CommandPrompt::Change_Instrument
		){
			promptState.isCommandWindowVisible = false;
		}

		if(toolbarState.selectedTool != constants::toolbar::Tool::Change_Instrument
		&& promptState.activeCommandPrompt == constants::prompts::CommandPrompt::Change_Instrument
		){
			promptState.isCommandWindowVisible = false;
		}


		if(toolbarState.selectedTool == constants::toolbar::Tool::Change_Instrument){
			GuiSetState(STATE_PRESSED);
		}else{
			GuiSetState(context.machine.isPlaying ? STATE_DISABLED : STATE_NORMAL);
		}

		if(GuiButton(calculateBoundsAtAnchor(anchor, bounds.openInstrumentWindowButton), nullptr)){
			promptState.activeCommandPrompt = constants::prompts::CommandPrompt::Change_Instrument;
			promptState.isCommandWindowVisible = true;

			toolbarState.selectedTool = constants::toolbar::Tool::Change_Instrument;
		}
		
		GuiSetState(STATE_NORMAL);
	}

	const bool hasSelectionArea{
		context.interface.clipboard.selectionArea.has_value()
	 && context.interface.clipboard.selectionArea.value().widthInCells > 0
	 && context.interface.clipboard.selectionArea.value().heightInCells > 0
	};
	const bool hasCopiedSelection{context.interface.clipboard.hasCopiedSelection};
	const bool isPasteModeEnabled{context.interface.clipboard.isPasteModeEnabled};

	auto drawButton{[&](const Rectangle &buttonBounds, const char *buttonText, bool isEnabled){
		if(!isEnabled) GuiSetState(STATE_DISABLED);
		const bool isPressed{GuiButton(buttonBounds, buttonText) != 0};
		GuiSetState(STATE_NORMAL);
		return isEnabled && isPressed;
	}};

	const bool isCutEnabled{!isSystemChannelSelected && !isPasteModeEnabled && hasSelectionArea};
	const bool isCopyEnabled{!isSystemChannelSelected && !isPasteModeEnabled && hasSelectionArea};
	const bool isPasteEnabled{!isSystemChannelSelected && hasCopiedSelection && !context.machine.isPlaying};

	context.interface.toolbar.isCutNoteButtonPressed = drawButton(
		calculateBoundsAtAnchor(anchor, bounds.cutNoteButton),
		toolbar_constants::CutNoteButtonText,
		isCutEnabled
	);
	context.interface.toolbar.isCopyNoteButtonPressed = drawButton(
		calculateBoundsAtAnchor(anchor, bounds.copyNoteButton),
		toolbar_constants::CopyNoteButtonText,
		isCopyEnabled
	);
	context.interface.toolbar.isPasteNoteButtonPressed = false;

	const bool isDisabledPasteToggle{context.interface.clipboard.isPasteModeEnabled};
	if(!isPasteEnabled) GuiDisable();
	GuiToggle(
		calculateBoundsAtAnchor(anchor, bounds.pasteNoteButton),
		toolbar_constants::PasteNoteButtonText,
		&context.interface.clipboard.isPasteModeEnabled
	);
	if(!isPasteEnabled) GuiEnable();
}