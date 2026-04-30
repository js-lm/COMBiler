#include "prompts.hpp"

#include <raygui.h>

#include <algorithm>

#include "constants.hpp"

#include "labels.hpp"
#include "interface/utilities.hpp"

using namespace interface;
namespace prompts_labels = constants::labels::prompts;
namespace manager_constants = constants::prompts::constants_manager;

void Prompts::drawConstantsManagerPrompt(program_states::InterfaceContext &context){
	auto &state{context.interface.prompts};
	const auto &anchor{context.layout.anchor.prompts.constantsManagerWindow};
	const auto &bounds{context.layout.bounds.prompts.constantsManager};

	const bool isOverlayPromptVisible{
        state.isConstantsManagerWarningWindowVisible
     || state.isConstantsManagerInfoWindowVisible
	};

	if(isOverlayPromptVisible) GuiLock();

	const Rectangle windowBounds{calculateBoundsAtAnchor(anchor, bounds.windowBox)};
	if(GuiWindowBox(windowBounds, prompts_labels::ConstantsManagerWindowBoxText)){
		state.isConstantsManagerWindowVisible = false;
		if(isOverlayPromptVisible){
			GuiUnlock();
		}
		return;
	}

// left
	GuiStatusBar(calculateBoundsAtAnchor(anchor, bounds.idBar), prompts_labels::IDBarText);
	GuiStatusBar(calculateBoundsAtAnchor(anchor, bounds.nameBar), prompts_labels::NameBarText);
	GuiStatusBar(calculateBoundsAtAnchor(anchor, bounds.typeBar), prompts_labels::TypeBarText);
	GuiStatusBar(calculateBoundsAtAnchor(anchor, bounds.valueBar), prompts_labels::ValueBarText);

	auto &scrollOffset{state.constantsManagerScrollPanelScrollOffset};
	auto &view{state.constantsManagerScrollPanelScrollView};

	GuiScrollPanel(
		calculateBoundsAtAnchor(anchor, bounds.constantsScrollPanel),
		nullptr,
		calculateBoundsAtAnchor(anchor, bounds.constantsContentPanel),
		&scrollOffset,
		&view
	);

	auto boundAtAnchorWithScrollOffset{[&](Rectangle bounds, size_t index)->Rectangle{
		bounds.y += scrollOffset.y;
		bounds.y += context.layout.bounds.prompts.constantsManager.constantItemPanel.height * index;
		return calculateBoundsAtAnchor(anchor, bounds);
	}};

	// WIP
	BeginScissorMode(view.x, view.y, view.width, view.height); {

		const auto previousWidth{GuiGetStyle(BUTTON, BORDER_WIDTH)};
		GuiSetStyle(BUTTON, BORDER_WIDTH, 1);

		for(size_t itemIndex{0}; itemIndex < constants::project_data::MaximumNumberOfConstants; itemIndex++){
			GuiButton(boundAtAnchorWithScrollOffset(bounds.constantItemPanel, itemIndex), nullptr);
			// GuiPanel(calculateBoundsAtAnchor(anchor, bounds.constantsPanel), nullptr);
			GuiLabel(boundAtAnchorWithScrollOffset(bounds.listIdLabel, itemIndex), TextFormat("%02d", itemIndex));
			GuiLabel(boundAtAnchorWithScrollOffset(bounds.listTypeLabel, itemIndex), "#122#Volume");
			GuiLabel(boundAtAnchorWithScrollOffset(bounds.listValueLabel, itemIndex), "Target: Ch3, Value: 8");
			GuiLabel(boundAtAnchorWithScrollOffset(bounds.listNameLabel, itemIndex), "Main Volume");
		}

		GuiSetStyle(BUTTON, BORDER_WIDTH, previousWidth);

	} EndScissorMode();

// right
	GuiGroupBox(calculateBoundsAtAnchor(anchor, bounds.editConstantGroupBox), prompts_labels::EditNameGroupBoxText);
	GuiLabel(calculateBoundsAtAnchor(anchor, bounds.textConstantLabel), prompts_labels::TextConstantLabelText);
	if(GuiTextBox(
		calculateBoundsAtAnchor(anchor, bounds.editConstantTextBox),
		state.constantsManagerNameTextBoxText,
		manager_constants::ConstantNameTextMaximumLength,
		state.constantsManagerNameTextBoxEditMode
	)){
		state.constantsManagerNameTextBoxEditMode = !state.constantsManagerNameTextBoxEditMode;
	}

	GuiToggleGroup(
		calculateBoundsAtAnchor(anchor, bounds.typeToggleGroup),
		prompts_labels::TypeToggleGroupText,
		&state.constantsManagerTypeToggleGroupIndex
	);
	state.constantsManagerTypeToggleGroupIndex = std::clamp(
		state.constantsManagerTypeToggleGroupIndex,
		0, manager_constants::TypeCount - 1
	);

	const auto selectedConstantType{manager_constants::typeFromIndex(state.constantsManagerTypeToggleGroupIndex)};
	const bool isTempoSectionVisible{selectedConstantType == manager_constants::Type::Tempo};
	const bool isVolumeSectionVisible{selectedConstantType == manager_constants::Type::Volume};
	const bool isArticulationSectionVisible{selectedConstantType == manager_constants::Type::Articulation};
	const bool isTargetSectionVisible{isVolumeSectionVisible || isArticulationSectionVisible};

	if(isTempoSectionVisible){
		GuiGroupBox(calculateBoundsAtAnchor(anchor, bounds.tempoGroupBox), prompts_labels::ConstantsManagerTempoGroupBoxText);
		GuiSlider(
			calculateBoundsAtAnchor(anchor, bounds.tempoSlider),
			prompts_labels::ConstantsManagerTempoSliderText,
			nullptr,
			&state.constantsManagerTempoSliderValue,
			constants::prompts::TempoPercentageMinimumValue,
			constants::prompts::TempoPercentageMaximumValue
		);
	}else if(isTargetSectionVisible){
		GuiGroupBox(calculateBoundsAtAnchor(anchor, bounds.targetGroupBox), prompts_labels::TargetGroupBoxText);
		GuiLabel(calculateBoundsAtAnchor(anchor, bounds.targetChannelLabel), prompts_labels::TargetChannelLabelText);
		GuiToggleGroup(
			calculateBoundsAtAnchor(anchor, bounds.targetToggleGroup),
			prompts_labels::TargetToggleGroupText,
			&state.constantsManagerTargetToggleGroupIndex
		);
	}

	if(isArticulationSectionVisible){
		GuiGroupBox(
			calculateBoundsAtAnchor(anchor, bounds.articulationGroupBox),
			prompts_labels::ConstantsManagerArticulationGroupBoxText
		);
		GuiToggleGroup(
			calculateBoundsAtAnchor(anchor, bounds.articulationToggleGroup),
			prompts_labels::ConstantsManagerArticulationToggleGroupText,
			&state.constantsManagerArticulationToggleGroupIndex
		);
	}else if(isVolumeSectionVisible){
		GuiGroupBox(calculateBoundsAtAnchor(anchor, bounds.volumeGroupBox), prompts_labels::ConstantsManagerVolumeGroupBoxText);
		GuiLabel(calculateBoundsAtAnchor(anchor, bounds.volumeLabel), prompts_labels::VolumeLabelText);
		GuiToggleGroup(
			calculateBoundsAtAnchor(anchor, bounds.volumeToggleGroup),
			prompts_labels::ConstantsManagerVolumeToggleGroupText,
			&state.constantsManagerVolumeToggleGroupIndex
		);
	}

	GuiGroupBox(calculateBoundsAtAnchor(anchor, bounds.swapGroupBox), prompts_labels::SwapGroupBoxText);
	GuiLabel(calculateBoundsAtAnchor(anchor, bounds.swapLeftLabel), prompts_labels::SwapLeftLabelText);
	GuiLabel(calculateBoundsAtAnchor(anchor, bounds.swapRightLabel), prompts_labels::SwapRightLabelText);

	if(GuiTextBox(
		calculateBoundsAtAnchor(anchor, bounds.swapLeftTextBox),
		state.constantsManagerSwapLeftTextBoxText,
		manager_constants::SwapIndexTextMaximumLength,
		state.constantsManagerSwapLeftTextBoxEditMode
	)){
		state.constantsManagerSwapLeftTextBoxEditMode = !state.constantsManagerSwapLeftTextBoxEditMode;
	}

	if(GuiTextBox(
		calculateBoundsAtAnchor(anchor, bounds.swapRightTextBox),
		state.constantsManagerSwapRightTextBoxText,
		manager_constants::SwapIndexTextMaximumLength,
		state.constantsManagerSwapRightTextBoxEditMode
	)){
		state.constantsManagerSwapRightTextBoxEditMode = !state.constantsManagerSwapRightTextBoxEditMode;
	}

	GuiButton(calculateBoundsAtAnchor(anchor, bounds.commitButton), prompts_labels::ConstantsManagerCommitButtonText);
	GuiButton(calculateBoundsAtAnchor(anchor, bounds.revertButton), prompts_labels::ConstantsManagerRevertButtonText);
	GuiButton(calculateBoundsAtAnchor(anchor, bounds.swapButton), prompts_labels::SwapButtonText);
	GuiButton(calculateBoundsAtAnchor(anchor, bounds.loadButton), prompts_labels::LoadButtonText);

	if(isOverlayPromptVisible) GuiUnlock();
}

