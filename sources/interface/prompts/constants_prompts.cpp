#include "prompts.hpp"

#include <raygui.h>

#include <algorithm>

#include "constants.hpp"

#include "labels.hpp"
#include "interface/utilities.hpp"

#include <magic_enum/magic_enum.hpp>

#include <fmt/format.h>

#include <debug_utilities.hpp>

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
	if(GuiWindowBox(
		windowBounds, 
		state.hasModified ? prompts_labels::ConstantsManagerWindowBoxUnsavedText : prompts_labels::ConstantsManagerWindowBoxText
	)){
		state.isConstantsManagerWindowVisible = false;

		if(isOverlayPromptVisible) GuiUnlock();

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


	if(auto projectData{context.system.project.data.lock()}){

		const auto &commandList{projectData->data->commandPalette.getList()};

		bool isRightPanelDirty{false};

		const auto previousWidth{GuiGetStyle(BUTTON, BORDER_WIDTH)};
		GuiSetStyle(BUTTON, BORDER_WIDTH, 0);

		BeginScissorMode(view.x, view.y, view.width, view.height); {

			for(size_t itemIndex{0}; 
				itemIndex < constants::project_data::MaximumNumberOfConstants; 
				itemIndex++
			){

				if(itemIndex == state.draft.constantIndex){
					GuiSetStyle(BUTTON, BORDER_WIDTH, previousWidth);
				}

				auto boundAtAnchorWithScrollOffset{[&](Rectangle bounds)->Rectangle{
					bounds.y += scrollOffset.y;
					bounds.y += context.layout.bounds.prompts.constantsManager.constantItemPanel.height * itemIndex;
					return calculateBoundsAtAnchor(anchor, bounds);
				}};

				const auto &commandPair{commandList[itemIndex]};

				if(GuiButton(boundAtAnchorWithScrollOffset(bounds.constantItemPanel), nullptr)){

					if(state.hasModified){
						state.isConstantsManagerWarningWindowVisible = true;

						GuiSetStyle(BUTTON, BORDER_WIDTH, previousWidth);

						state.requestedConstantIndex = itemIndex;

						return;
					}else{
						state.draft.constantIndex = itemIndex;
						isRightPanelDirty = true;
					}

				}

				std::string nameLabel{prompts_labels::DefaultNameLabel};
				std::string typeLabel{prompts_labels::DefaultTypeLabel};
				std::string valueLabel{prompts_labels::DefaultValueLabel};

				if(commandPair.second){
					nameLabel = commandPair.first;

					std::visit([&](const auto &command){
						using Type = std::decay_t<decltype(command)>;

						if constexpr(std::is_same_v<command::Tempo, Type>){
							typeLabel = prompts_labels::TempoTypeLabel;
							valueLabel = fmt::format("{}", command.tempo);

						}else if constexpr(std::is_same_v<command::Volume, Type>){
							typeLabel = prompts_labels::VolumeTypeLabel;

							valueLabel = fmt::format("{}; {}", magic_enum::enum_name(command.target), command.volume);
							
						}else if constexpr(std::is_same_v<command::Articulation, Type>){
							typeLabel = prompts_labels::ArticulationTypeLabel;
							valueLabel = fmt::format("{}; {}", magic_enum::enum_name(command.target), magic_enum::enum_name(command.articulation));
						}

					}, commandPair.second.value());
				}

				GuiLabel(boundAtAnchorWithScrollOffset(bounds.listIdLabel), TextFormat("%02d", itemIndex));
				GuiLabel(boundAtAnchorWithScrollOffset(bounds.listNameLabel), nameLabel.c_str());
				GuiLabel(boundAtAnchorWithScrollOffset(bounds.listTypeLabel), typeLabel.c_str());
				GuiLabel(boundAtAnchorWithScrollOffset(bounds.listValueLabel), valueLabel.c_str());
			
				GuiSetStyle(BUTTON, BORDER_WIDTH, 0);
			}

			GuiSetStyle(BUTTON, BORDER_WIDTH, previousWidth);

		} EndScissorMode();



		if(isRightPanelDirty){
			isRightPanelDirty = false;

			state.draft.reset(state.draft.constantIndex);

			if(auto &command{commandList[state.draft.constantIndex].second}){

				std::visit([&state](const auto &command){
					using Type = std::decay_t<decltype(command)>;

					if constexpr(std::is_same_v<command::Tempo, Type>){
						state.draft.commandType = constants::prompts::CommandPrompt::Tempo;

						state.draft.tempoSliderValue = command.tempo;

					}else if constexpr(std::is_same_v<command::Volume, Type>){
						state.draft.commandType = constants::prompts::CommandPrompt::Volume;

						state.draft.targetToggleGroupIndex = static_cast<int>(command.target);
						state.draft.volumeToggleGroupIndex = command.volume;


					}else if constexpr(std::is_same_v<command::Articulation, Type>){
						state.draft.commandType = constants::prompts::CommandPrompt::Articulation;

						state.draft.targetToggleGroupIndex = static_cast<int>(command.target);
						state.draft.articulationToggleGroupIndex = static_cast<int>(command.articulation);

					}else{
						state.draft.commandType = std::nullopt;

					}

				}, command.value());

			}



		}


	}


// right

	const std::string previousName{state.draft.nameTextBoxText};
	const int previousType{state.constantsManagerTypeToggleGroupIndex};
	const float previousTempo{state.draft.tempoSliderValue};
	const int previousTarget{state.draft.targetToggleGroupIndex};
	const int previousArticulation{state.draft.articulationToggleGroupIndex};
	const int previousVolume{state.draft.volumeToggleGroupIndex};
	const std::string previousSwapLeft{state.draft.swapLeftTextBoxText};
	const std::string previousSwapRight{state.draft.swapRightTextBoxText};

	GuiGroupBox(calculateBoundsAtAnchor(anchor, bounds.editConstantGroupBox), prompts_labels::EditNameGroupBoxText);
	GuiLabel(calculateBoundsAtAnchor(anchor, bounds.textConstantLabel), prompts_labels::TextConstantLabelText);
	if(GuiTextBox(
		calculateBoundsAtAnchor(anchor, bounds.editConstantTextBox),
		state.draft.nameTextBoxText,
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
			&state.draft.tempoSliderValue,
			constants::prompts::TempoPercentageMinimumValue,
			constants::prompts::TempoPercentageMaximumValue
		);
	}else if(isTargetSectionVisible){
		GuiGroupBox(calculateBoundsAtAnchor(anchor, bounds.targetGroupBox), prompts_labels::TargetGroupBoxText);
		GuiLabel(calculateBoundsAtAnchor(anchor, bounds.targetChannelLabel), prompts_labels::TargetChannelLabelText);
		GuiToggleGroup(
			calculateBoundsAtAnchor(anchor, bounds.targetToggleGroup),
			prompts_labels::TargetToggleGroupText,
			&state.draft.targetToggleGroupIndex
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
			&state.draft.articulationToggleGroupIndex
		);
	}else if(isVolumeSectionVisible){
		GuiGroupBox(calculateBoundsAtAnchor(anchor, bounds.volumeGroupBox), prompts_labels::ConstantsManagerVolumeGroupBoxText);
		GuiLabel(calculateBoundsAtAnchor(anchor, bounds.volumeLabel), prompts_labels::VolumeLabelText);
		GuiToggleGroup(
			calculateBoundsAtAnchor(anchor, bounds.volumeToggleGroup),
			prompts_labels::ConstantsManagerVolumeToggleGroupText,
			&state.draft.volumeToggleGroupIndex
		);
	}

	GuiGroupBox(calculateBoundsAtAnchor(anchor, bounds.swapGroupBox), prompts_labels::SwapGroupBoxText);
	GuiLabel(calculateBoundsAtAnchor(anchor, bounds.swapLeftLabel), prompts_labels::SwapLeftLabelText);
	GuiLabel(calculateBoundsAtAnchor(anchor, bounds.swapRightLabel), prompts_labels::SwapRightLabelText);

	if(GuiTextBox(
		calculateBoundsAtAnchor(anchor, bounds.swapLeftTextBox),
		state.draft.swapLeftTextBoxText,
		manager_constants::SwapIndexTextMaximumLength,
		state.constantsManagerSwapLeftTextBoxEditMode
	)){
		state.constantsManagerSwapLeftTextBoxEditMode = !state.constantsManagerSwapLeftTextBoxEditMode;
	}

	if(GuiTextBox(
		calculateBoundsAtAnchor(anchor, bounds.swapRightTextBox),
		state.draft.swapRightTextBoxText,
		manager_constants::SwapIndexTextMaximumLength,
		state.constantsManagerSwapRightTextBoxEditMode
	)){
		state.constantsManagerSwapRightTextBoxEditMode = !state.constantsManagerSwapRightTextBoxEditMode;
	}

	if(previousName != state.draft.nameTextBoxText
	|| previousType != state.constantsManagerTypeToggleGroupIndex
	|| previousTempo != state.draft.tempoSliderValue
	|| previousTarget != state.draft.targetToggleGroupIndex
	|| previousArticulation != state.draft.articulationToggleGroupIndex
	|| previousVolume != state.draft.volumeToggleGroupIndex
	|| previousSwapLeft != state.draft.swapLeftTextBoxText
	|| previousSwapRight != state.draft.swapRightTextBoxText)
	{
		state.hasModified = true;
	}

	if(GuiButton(
		calculateBoundsAtAnchor(anchor, bounds.commitButton), 
		state.hasModified ? prompts_labels::ConstantsManagerCommitButtonUnsavedText 
				: prompts_labels::ConstantsManagerCommitButtonText
	)){ commit(context);}
	GuiButton(calculateBoundsAtAnchor(anchor, bounds.revertButton), prompts_labels::ConstantsManagerRevertButtonText);
	GuiButton(calculateBoundsAtAnchor(anchor, bounds.swapButton), prompts_labels::SwapButtonText);
	
	if(GuiButton(calculateBoundsAtAnchor(anchor, bounds.loadButton), prompts_labels::LoadButtonText)){


		if(auto projectData{context.system.project.data.lock()}){

			const auto &commandList{projectData->data->commandPalette.getList()};

			if(commandList[state.draft.constantIndex].second){
				load(context);
			}else{
				state.isConstantsManagerInfoWindowVisible = true;
			}

		}
	}

	if(isOverlayPromptVisible) GuiUnlock();

	drawConstantsManagerCommitPrompt(context);
	drawConstantsManagerNotDefinedPrompt(context);

	DEBUG_PRINT_IF_CHANGED("state.hasModified:{}", state.hasModified);
}

void Prompts::drawConstantsManagerCommitPrompt(program_states::InterfaceContext &context){
	auto &state{context.interface.prompts};

	if(!state.isConstantsManagerWarningWindowVisible) return;


	const auto &bounds{context.layout.bounds.prompts.constantsManagerWarning};
	Rectangle centeredBounds{
		(static_cast<float>(context.system.window.interfaceRenderTextureWidth) - bounds.windowBox.width) / 2.0f,
		(static_cast<float>(context.system.window.interfaceRenderTextureHeight) - bounds.windowBox.height) / 2.0f,
		bounds.windowBox.width,
		bounds.windowBox.height
	};

	std::string buttons{fmt::format("{};{}", prompts_labels::WarningYesButtonText, prompts_labels::WarningNoButtonText)};
	int result{GuiMessageBox(
		centeredBounds, 
		prompts_labels::WarningWindowBoxText, 
		prompts_labels::WarningLabelText, 
		buttons.c_str()
	)};

	if(result == 1){ // TODO: magic numbers
		state.isConstantsManagerWarningWindowVisible = false;
		commit(context);
		state.draft.constantIndex = state.requestedConstantIndex;
	}else if(result == 2){
		state.isConstantsManagerWarningWindowVisible = false;
		state.draft.constantIndex = state.requestedConstantIndex;
	}else if(result == 0 || IsKeyPressed(KEY_ESCAPE)){
		state.isConstantsManagerWarningWindowVisible = false;
	}
}

void Prompts::drawConstantsManagerNotDefinedPrompt(program_states::InterfaceContext &context){
	auto &state{context.interface.prompts};

	if(!state.isConstantsManagerInfoWindowVisible) return;

	const auto &bounds{context.layout.bounds.prompts.constantsManagerInfo};
	Rectangle centeredBounds{
		(static_cast<float>(context.system.window.interfaceRenderTextureWidth) - bounds.windowBox.width) / 2.0f,
		(static_cast<float>(context.system.window.interfaceRenderTextureHeight) - bounds.windowBox.height) / 2.0f,
		bounds.windowBox.width,
		bounds.windowBox.height
	};

	int result{GuiMessageBox(
		centeredBounds, 
		prompts_labels::InfoWindowBoxText, 
		prompts_labels::InfoLabelText, 
		prompts_labels::InfoOKButtonText
	)};

	if(result >= 0 || IsKeyPressed(KEY_ESCAPE)){
		state.isConstantsManagerInfoWindowVisible = false;
	}
}