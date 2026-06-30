#include "prompts.hpp"

#include <raygui.h>

#include <algorithm>
#include <cmath>
#include <string>

#include "constants.hpp"

#include "labels.hpp"
#include "interface/utilities.hpp"

#include "debug_utilities.hpp"

#include <magic_enum/magic_enum.hpp>

using namespace interface;
namespace prompts_constants = constants::labels::prompts;

void Prompts::draw(program_states::InterfaceContext &context){
    auto &promptState{context.interface.prompts};
    const bool isAnyPromptVisible{
        promptState.isCommandWindowVisible
     || promptState.isConstantsManagerWindowVisible
     || promptState.isOverwritePromptVisible
    };

    if(!isAnyPromptVisible) return;

    const Rectangle overlayBounds{
        0, 0,
        static_cast<float>(context.system.window.interfaceRenderTextureWidth),
        static_cast<float>(context.system.window.interfaceRenderTextureHeight)
    };
    DrawRectangleRec(overlayBounds, Fade(BLACK, constants::prompts::OverlayDimAlpha));

    if(promptState.isConstantsManagerWindowVisible){
        drawConstantsManagerPrompt(context);
        return;
    }

    if(promptState.isOverwritePromptVisible){
        drawOverwritePrompt(context);
        return;
    }

    if(promptState.activeCommandPrompt == constants::prompts::CommandPrompt::Tempo){
        drawTempoPrompt(context);
        return;
    }

    if(promptState.activeCommandPrompt == constants::prompts::CommandPrompt::Volume){
        drawVolumePrompt(context);
        return;
    }

    if(promptState.activeCommandPrompt == constants::prompts::CommandPrompt::Articulation){
        drawArticulationPrompt(context);
        return;
    }

    drawInstrumentPrompt(context);
}

void Prompts::drawTempoPrompt(program_states::InterfaceContext &context){
    auto &promptState{context.interface.prompts};
    const auto &promptBounds{context.layout.bounds.prompts.tempo};

    const Rectangle windowBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.windowBox)};
    if(GuiWindowBox(windowBounds, prompts_constants::TempoWindowBoxText)){
        promptState.isCommandWindowVisible = false;
        return;
    }

    const Rectangle tempoGroupBoxBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.groupBox)};
    const Rectangle tempoSliderBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.slider)};
    const Rectangle tempoValueBoxBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.percentageValueBox)};
    const Rectangle tempoPercentageLabelBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.percentageLabel)};
    const Rectangle tempoDescriptionLineBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.descriptionLine)};
    const Rectangle tempoDescriptionLabelBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.descriptionLabel)};
    const Rectangle backButtonBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.backButton)};
    const Rectangle confirmButtonBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.confirmButton)};

    GuiGroupBox(tempoGroupBoxBounds, prompts_constants::TempoGroupBoxText);

    GuiSlider(
        tempoSliderBounds,
        prompts_constants::TempoSliderText,
        nullptr,
        &promptState.tempoPlaybackSpeedPercentage,
        constants::prompts::TempoPercentageMinimumValue,
        constants::prompts::TempoPercentageMaximumValue
    );

    // TODO: should I just set the min/max to 1/50 and just multiply it by 2 instead?
    if(int percentageInteger{static_cast<int>(std::round(promptState.tempoPlaybackSpeedPercentage))}; percentageInteger % 2 != 0){
        promptState.tempoPlaybackSpeedPercentage = --percentageInteger;
    }

    promptState.tempoPercentageValue = static_cast<int>(std::round(promptState.tempoPlaybackSpeedPercentage));
    if(GuiValueBox(
        tempoValueBoxBounds,
        nullptr,
        &promptState.tempoPercentageValue,
        constants::prompts::TempoPercentageMinimumValue,
        constants::prompts::TempoPercentageMaximumValue,
        promptState.tempoPercentageValueBoxEditMode
    )){
        promptState.tempoPercentageValueBoxEditMode = !promptState.tempoPercentageValueBoxEditMode;
    }

    promptState.tempoPercentageValue = std::clamp(
        promptState.tempoPercentageValue,
        constants::prompts::TempoPercentageMinimumValue,
        constants::prompts::TempoPercentageMaximumValue
    );
    promptState.tempoPlaybackSpeedPercentage = static_cast<float>(promptState.tempoPercentageValue);

    // DEBUG_PRINT_IF_CHANGED(
    //     "tempoPlaybackSpeedPercentage: {}, tempoPercentageValue: {}",
    //     promptState.tempoPlaybackSpeedPercentage,
    //     promptState.tempoPercentageValue
    // );

    GuiLabel(tempoPercentageLabelBounds, prompts_constants::TempoPercentageLabelText);
    GuiLine(tempoDescriptionLineBounds, nullptr);
    GuiLabel(tempoDescriptionLabelBounds, prompts_constants::TempoDescriptionLabelText);

    const bool isBackButtonPressed{static_cast<bool>(GuiButton(backButtonBounds, prompts_constants::BackButtonText))};
    const bool isConfirmButtonPressed{static_cast<bool>(GuiButton(confirmButtonBounds, prompts_constants::ConfirmButtonText))};

    if(isBackButtonPressed || isConfirmButtonPressed){
        promptState.isCommandWindowVisible = false;
    }
}

void Prompts::drawVolumePrompt(program_states::InterfaceContext &context){
    auto &promptState{context.interface.prompts};
    const auto &promptBounds{context.layout.bounds.prompts.volume};

    const Rectangle windowBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.windowBox)};
    if(GuiWindowBox(windowBounds, prompts_constants::VolumeWindowBoxText)){
        promptState.isCommandWindowVisible = false;
        return;
    }

    const Rectangle volumeGroupBoxBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.groupBox)};
    const Rectangle volumeLevelToggleGroupBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.levelToggleGroup)};
    const Rectangle volumeTargetGroupBoxBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.targetGroupBox)};
    const Rectangle volumeTargetLabelBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.targetLabel)};
    const Rectangle volumeTargetToggleGroupBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.targetToggleGroup)};
    const Rectangle volumeDescriptionLabelBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.descriptionLabel)};
    const Rectangle backButtonBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.backButton)};
    const Rectangle confirmButtonBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.confirmButton)};

    GuiGroupBox(volumeGroupBoxBounds, prompts_constants::VolumeGroupBoxText);
    GuiToggleGroup(volumeLevelToggleGroupBounds, prompts_constants::VolumeLevelToggleGroupText, &promptState.volumeLevelToggleIndex);

    GuiGroupBox(volumeTargetGroupBoxBounds, prompts_constants::VolumeTargetGroupBoxText);
    GuiLabel(volumeTargetLabelBounds, prompts_constants::VolumeTargetLabelText);
    GuiToggleGroup(volumeTargetToggleGroupBounds, prompts_constants::VolumeTargetToggleGroupText, &promptState.volumeTargetToggleIndex);

    GuiLabel(volumeDescriptionLabelBounds, prompts_constants::VolumeDescriptionLabelText);

    const bool isBackButtonPressed{static_cast<bool>(GuiButton(backButtonBounds, prompts_constants::BackButtonText))};
    const bool isConfirmButtonPressed{static_cast<bool>(GuiButton(confirmButtonBounds, prompts_constants::ConfirmButtonText))};

    if(isBackButtonPressed || isConfirmButtonPressed){
        promptState.isCommandWindowVisible = false;
    }
}

void Prompts::drawArticulationPrompt(program_states::InterfaceContext &context){
    auto &promptState{context.interface.prompts};
    const auto &promptBounds{context.layout.bounds.prompts.articulation};

    const Rectangle windowBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.windowBox)};
    if(GuiWindowBox(windowBounds, prompts_constants::ArticulationWindowBoxText)){
        promptState.isCommandWindowVisible = false;
        return;
    }

    const Rectangle articulationGroupBoxBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.groupBox)};
    const Rectangle articulationStateToggleGroupBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.stateToggleGroup)};
    const Rectangle articulationLineBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.line)};
    const Rectangle articulationStateLabelBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.stateLabel)};
    const Rectangle articulationDescriptionLabelBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.descriptionLabel)};

    const Rectangle articulationTargetGroupBoxBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.targetGroupBox)};
    const Rectangle articulationTargetToggleGroupBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.targetToggleGroup)};

    const Rectangle backButtonBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.backButton)};
    const Rectangle confirmButtonBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.confirmButton)};

    GuiGroupBox(articulationGroupBoxBounds, prompts_constants::ArticulationGroupBoxText);
    GuiToggleGroup(articulationStateToggleGroupBounds, prompts_constants::ArticulationStateToggleGroupText, &promptState.articulationStateToggleIndex);

    GuiLine(articulationLineBounds, nullptr);
    GuiLabel(articulationStateLabelBounds, prompts_constants::ArticulationStateLabelText);
    GuiLabel(articulationDescriptionLabelBounds, prompts_constants::ArticulationDescriptionLabelText);

    GuiGroupBox(articulationTargetGroupBoxBounds, prompts_constants::ArticulationTargetGroupBoxText);
    GuiToggleGroup(articulationTargetToggleGroupBounds, prompts_constants::ArticulationTargetToggleGroupText, &promptState.articulationTargetToggleIndex);

    const bool isBackButtonPressed{static_cast<bool>(GuiButton(backButtonBounds, prompts_constants::BackButtonText))};
    const bool isConfirmButtonPressed{static_cast<bool>(GuiButton(confirmButtonBounds, prompts_constants::ConfirmButtonText))};

    if(isBackButtonPressed || isConfirmButtonPressed){
        promptState.isCommandWindowVisible = false;
    }
}

void Prompts::drawInstrumentPrompt(program_states::InterfaceContext &context){
    auto &promptState{context.interface.prompts};
    const auto &promptBounds{context.layout.bounds.prompts.instrument};

    const Rectangle windowBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.windowBox)};
    if(GuiWindowBox(windowBounds, prompts_constants::InstrumentWindowBoxText)){
        promptState.isCommandWindowVisible = false;
        return;
    }

    promptState.instrumentFamilyListViewIndex = std::clamp(
        promptState.instrumentFamilyListViewIndex, 0,
        static_cast<int>(magic_enum::enum_count<music_data::InstrumentFamily>()) - 1
    );

    GuiListView(
        calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.familyListView),
        prompts_constants::InstrumentFamilyListViewText,
        &promptState.instrumentFamilyListViewScrollIndex,
        &promptState.instrumentFamilyListViewIndex
    );

    promptState.instrumentFamilyListViewIndex = std::clamp(
        promptState.instrumentFamilyListViewIndex, 0,
        static_cast<int>(magic_enum::enum_count<music_data::InstrumentFamily>()) - 1
    );
    promptState.selectedInstrumentFamily = static_cast<music_data::InstrumentFamily>(promptState.instrumentFamilyListViewIndex);

    const auto instrumentOptions{constants::prompts::getInstruments(promptState.selectedInstrumentFamily)};
    const std::string instrumentMembersText{instrumentListViewText(instrumentOptions)};

    if(instrumentOptions.empty()){
        promptState.instrumentMemberListViewIndex = 0;
    }else{
        promptState.instrumentMemberListViewIndex = std::clamp(
            promptState.instrumentMemberListViewIndex, 0,
            static_cast<int>(instrumentOptions.size()) - 1
        );

        promptState.selectedInstrument = instrumentOptions[static_cast<size_t>(promptState.instrumentMemberListViewIndex)];
    }

    GuiListView(
        calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.memberListView),
        instrumentMembersText.c_str(),
        &promptState.instrumentMemberListViewScrollIndex,
        &promptState.instrumentMemberListViewIndex
    );

    if(!instrumentOptions.empty()){
        promptState.instrumentMemberListViewIndex = std::clamp(
            promptState.instrumentMemberListViewIndex, 0,
            static_cast<int>(instrumentOptions.size()) - 1
        );
        promptState.selectedInstrument = instrumentOptions[static_cast<size_t>(promptState.instrumentMemberListViewIndex)];
    }

    GuiLabel(
        calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.familyLabel),
        prompts_constants::InstrumentFamilyLabelText
    );
    GuiLabel(
        calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.memberLabel),
        prompts_constants::InstrumentMemberLabelText
    );
}

void Prompts::drawOverwritePrompt(program_states::InterfaceContext &context){
    auto &promptState{context.interface.prompts};

    const auto anchor{context.layout.anchor.prompts.overwriteWarningWindow};
    const auto &bounds{context.layout.bounds.prompts.overwriteWarning};

    if(GuiWindowBox(calculateBoundsAtAnchor(anchor, bounds.windowBox), prompts_constants::FileDropWarningWindowBoxText)){
        promptState.isOverwritePromptVisible = false;
        promptState.overwriteAction = program_states::Interface::Prompts::OverwriteAction::None;
        return;
    }

    GuiLabel(calculateBoundsAtAnchor(anchor, bounds.label), prompts_constants::OverwriteWarningText);

    if(GuiButton(calculateBoundsAtAnchor(anchor, bounds.yesButton), prompts_constants::YesButtonText)){
        promptState.isOverwriteConfirmed = true;
        promptState.isOverwritePromptVisible = false;
    }
    if(GuiButton(calculateBoundsAtAnchor(anchor, bounds.noButton), prompts_constants::NoButtonText)){
        promptState.isOverwritePromptVisible = false;
        promptState.overwriteAction = program_states::Interface::Prompts::OverwriteAction::None;
    }
}
