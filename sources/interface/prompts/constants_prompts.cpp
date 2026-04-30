#include "prompts.hpp"

#include <raygui.h>

#include <algorithm>

#include "constants.hpp"

#include "labels.hpp"
#include "interface/utilities.hpp"

using namespace interface;
using namespace constants::labels::prompts; // TODO: forgot again

namespace manager_constants = constants::prompts::constants_manager;

void Prompts::drawConstantsManagerPrompt(program_states::InterfaceContext &context){
	auto &interfacePromptState{context.interface.prompts};
	const auto &anchor{context.layout.anchor.prompts.constantsManagerWindow};
	const auto &bounds{context.layout.bounds.prompts.constantsManager};

	const bool isOverlayPromptVisible{
        interfacePromptState.isConstantsManagerWarningWindowVisible
     || interfacePromptState.isConstantsManagerInfoWindowVisible
	};

	if(isOverlayPromptVisible) GuiLock();

	const Rectangle windowBounds{calculateBoundsAtAnchor(anchor, bounds.windowBox)};
	if(GuiWindowBox(windowBounds, ConstantsManagerWindowBoxText)){
		interfacePromptState.isConstantsManagerWindowVisible = false;
		if(isOverlayPromptVisible){
			GuiUnlock();
		}
		return;
	}

// left
	GuiStatusBar(calculateBoundsAtAnchor(anchor, bounds.idBar), IDBarText);
	GuiStatusBar(calculateBoundsAtAnchor(anchor, bounds.nameBar), NameBarText);
	GuiStatusBar(calculateBoundsAtAnchor(anchor, bounds.typeBar), TypeBarText);
	GuiStatusBar(calculateBoundsAtAnchor(anchor, bounds.valueBar), ValueBarText);

	auto &scrollOffset{interfacePromptState.constantsManagerScrollPanelScrollOffset};
	auto &view{interfacePromptState.constantsManagerScrollPanelScrollView};

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
	GuiGroupBox(calculateBoundsAtAnchor(anchor, bounds.editConstantGroupBox), EditNameGroupBoxText);
	GuiLabel(calculateBoundsAtAnchor(anchor, bounds.textConstantLabel), TextConstantLabelText);
	if(GuiTextBox(
		calculateBoundsAtAnchor(anchor, bounds.editConstantTextBox),
		interfacePromptState.constantsManagerNameTextBoxText,
		manager_constants::ConstantNameTextMaximumLength,
		interfacePromptState.constantsManagerNameTextBoxEditMode
	)){
		interfacePromptState.constantsManagerNameTextBoxEditMode = !interfacePromptState.constantsManagerNameTextBoxEditMode;
	}

	GuiToggleGroup(
		calculateBoundsAtAnchor(anchor, bounds.typeToggleGroup),
		TypeToggleGroupText,
		&interfacePromptState.constantsManagerTypeToggleGroupIndex
	);
	interfacePromptState.constantsManagerTypeToggleGroupIndex = std::clamp(
		interfacePromptState.constantsManagerTypeToggleGroupIndex,
		0, manager_constants::TypeCount - 1
	);

	const auto selectedConstantType{manager_constants::typeFromIndex(interfacePromptState.constantsManagerTypeToggleGroupIndex)};
	const bool isTempoSectionVisible{selectedConstantType == manager_constants::Type::Tempo};
	const bool isVolumeSectionVisible{selectedConstantType == manager_constants::Type::Volume};
	const bool isArticulationSectionVisible{selectedConstantType == manager_constants::Type::Articulation};
	const bool isTargetSectionVisible{isVolumeSectionVisible || isArticulationSectionVisible};

	if(isTempoSectionVisible){
		GuiGroupBox(calculateBoundsAtAnchor(anchor, bounds.tempoGroupBox), ConstantsManagerTempoGroupBoxText);
		GuiSlider(
			calculateBoundsAtAnchor(anchor, bounds.tempoSlider),
			ConstantsManagerTempoSliderText,
			nullptr,
			&interfacePromptState.constantsManagerTempoSliderValue,
			constants::prompts::TempoPercentageMinimumValue,
			constants::prompts::TempoPercentageMaximumValue
		);
	}else if(isTargetSectionVisible){
		GuiGroupBox(calculateBoundsAtAnchor(anchor, bounds.targetGroupBox), TargetGroupBoxText);
		GuiLabel(calculateBoundsAtAnchor(anchor, bounds.targetChannelLabel), TargetChannelLabelText);
		GuiToggleGroup(
			calculateBoundsAtAnchor(anchor, bounds.targetToggleGroup),
			TargetToggleGroupText,
			&interfacePromptState.constantsManagerTargetToggleGroupIndex
		);
	}

	if(isArticulationSectionVisible){
		GuiGroupBox(
			calculateBoundsAtAnchor(anchor, bounds.articulationGroupBox),
			ConstantsManagerArticulationGroupBoxText
		);
		GuiToggleGroup(
			calculateBoundsAtAnchor(anchor, bounds.articulationToggleGroup),
			ConstantsManagerArticulationToggleGroupText,
			&interfacePromptState.constantsManagerArticulationToggleGroupIndex
		);
	}else if(isVolumeSectionVisible){
		GuiGroupBox(calculateBoundsAtAnchor(anchor, bounds.volumeGroupBox), ConstantsManagerVolumeGroupBoxText);
		GuiLabel(calculateBoundsAtAnchor(anchor, bounds.volumeLabel), VolumeLabelText);
		GuiToggleGroup(
			calculateBoundsAtAnchor(anchor, bounds.volumeToggleGroup),
			ConstantsManagerVolumeToggleGroupText,
			&interfacePromptState.constantsManagerVolumeToggleGroupIndex
		);
	}

	GuiGroupBox(calculateBoundsAtAnchor(anchor, bounds.swapGroupBox), SwapGroupBoxText);
	GuiLabel(calculateBoundsAtAnchor(anchor, bounds.swapLeftLabel), SwapLeftLabelText);
	GuiLabel(calculateBoundsAtAnchor(anchor, bounds.swapRightLabel), SwapRightLabelText);

	if(GuiTextBox(
		calculateBoundsAtAnchor(anchor, bounds.swapLeftTextBox),
		interfacePromptState.constantsManagerSwapLeftTextBoxText,
		manager_constants::SwapIndexTextMaximumLength,
		interfacePromptState.constantsManagerSwapLeftTextBoxEditMode
	)){
		interfacePromptState.constantsManagerSwapLeftTextBoxEditMode = !interfacePromptState.constantsManagerSwapLeftTextBoxEditMode;
	}

	if(GuiTextBox(
		calculateBoundsAtAnchor(anchor, bounds.swapRightTextBox),
		interfacePromptState.constantsManagerSwapRightTextBoxText,
		manager_constants::SwapIndexTextMaximumLength,
		interfacePromptState.constantsManagerSwapRightTextBoxEditMode
	)){
		interfacePromptState.constantsManagerSwapRightTextBoxEditMode = !interfacePromptState.constantsManagerSwapRightTextBoxEditMode;
	}

	GuiButton(calculateBoundsAtAnchor(anchor, bounds.commitButton), ConstantsManagerCommitButtonText);
	GuiButton(calculateBoundsAtAnchor(anchor, bounds.revertButton), ConstantsManagerRevertButtonText);
	GuiButton(calculateBoundsAtAnchor(anchor, bounds.swapButton), SwapButtonText);
	GuiButton(calculateBoundsAtAnchor(anchor, bounds.loadButton), LoadButtonText);

	if(isOverlayPromptVisible) GuiUnlock();
}

