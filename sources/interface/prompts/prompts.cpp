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
using namespace constants::labels::prompts;

void Prompts::draw(program_states::InterfaceContext &context){
    auto &promptState{context.interface.prompts};
    if(!promptState.isCommandWindowVisible) return;

    const Rectangle overlayBounds{
        0, 0,
        static_cast<float>(context.system.window.interfaceRenderTextureWidth),
        static_cast<float>(context.system.window.interfaceRenderTextureHeight)
    };
    DrawRectangleRec(overlayBounds, Fade(BLACK, constants::prompts::OverlayDimAlpha));

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
    if(GuiWindowBox(windowBounds, TempoWindowBoxText)){
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

    GuiGroupBox(tempoGroupBoxBounds, TempoGroupBoxText);

    GuiSlider(
        tempoSliderBounds,
        TempoSliderText,
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

    GuiLabel(tempoPercentageLabelBounds, TempoPercentageLabelText);
    GuiLine(tempoDescriptionLineBounds, nullptr);
    GuiLabel(tempoDescriptionLabelBounds, TempoDescriptionLabelText);

    const bool isBackButtonPressed{static_cast<bool>(GuiButton(backButtonBounds, BackButtonText))};
    const bool isConfirmButtonPressed{static_cast<bool>(GuiButton(confirmButtonBounds, ConfirmButtonText))};

    if(isBackButtonPressed || isConfirmButtonPressed){
        promptState.isCommandWindowVisible = false;
    }
}

void Prompts::drawVolumePrompt(program_states::InterfaceContext &context){
    auto &promptState{context.interface.prompts};
    const auto &promptBounds{context.layout.bounds.prompts.volume};

    const Rectangle windowBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.windowBox)};
    if(GuiWindowBox(windowBounds, VolumeWindowBoxText)){
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

    GuiGroupBox(volumeGroupBoxBounds, VolumeGroupBoxText);
    GuiToggleGroup(volumeLevelToggleGroupBounds, VolumeLevelToggleGroupText, &promptState.volumeLevelToggleIndex);

    GuiGroupBox(volumeTargetGroupBoxBounds, VolumeTargetGroupBoxText);
    GuiLabel(volumeTargetLabelBounds, VolumeTargetLabelText);
    GuiToggleGroup(volumeTargetToggleGroupBounds, VolumeTargetToggleGroupText, &promptState.volumeTargetToggleIndex);

    GuiLabel(volumeDescriptionLabelBounds, VolumeDescriptionLabelText);

    const bool isBackButtonPressed{static_cast<bool>(GuiButton(backButtonBounds, BackButtonText))};
    const bool isConfirmButtonPressed{static_cast<bool>(GuiButton(confirmButtonBounds, ConfirmButtonText))};

    if(isBackButtonPressed || isConfirmButtonPressed){
        promptState.isCommandWindowVisible = false;
    }
}

void Prompts::drawArticulationPrompt(program_states::InterfaceContext &context){
    auto &promptState{context.interface.prompts};
    const auto &promptBounds{context.layout.bounds.prompts.articulation};

    const Rectangle windowBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.windowBox)};
    if(GuiWindowBox(windowBounds, ArticulationWindowBoxText)){
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

    GuiGroupBox(articulationGroupBoxBounds, ArticulationGroupBoxText);
    GuiToggleGroup(articulationStateToggleGroupBounds, ArticulationStateToggleGroupText, &promptState.articulationStateToggleIndex);

    GuiLine(articulationLineBounds, nullptr);
    GuiLabel(articulationStateLabelBounds, ArticulationStateLabelText);
    GuiLabel(articulationDescriptionLabelBounds, ArticulationDescriptionLabelText);

    GuiGroupBox(articulationTargetGroupBoxBounds, ArticulationTargetGroupBoxText);
    GuiToggleGroup(articulationTargetToggleGroupBounds, ArticulationTargetToggleGroupText, &promptState.articulationTargetToggleIndex);

    const bool isBackButtonPressed{static_cast<bool>(GuiButton(backButtonBounds, BackButtonText))};
    const bool isConfirmButtonPressed{static_cast<bool>(GuiButton(confirmButtonBounds, ConfirmButtonText))};

    if(isBackButtonPressed || isConfirmButtonPressed){
        promptState.isCommandWindowVisible = false;
    }
}

void Prompts::drawInstrumentPrompt(program_states::InterfaceContext &context){
    auto &promptState{context.interface.prompts};
    const auto &promptBounds{context.layout.bounds.prompts.instrument};

    const Rectangle windowBounds{calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.windowBox)};
    if(GuiWindowBox(windowBounds, InstrumentWindowBoxText)){
        promptState.isCommandWindowVisible = false;
        return;
    }

    promptState.instrumentFamilyListViewIndex = std::clamp(
        promptState.instrumentFamilyListViewIndex, 0,
        static_cast<int>(magic_enum::enum_count<music_data::InstrumentFamily>()) - 1
    );

    GuiListView(
        calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.familyListView),
        InstrumentFamilyListViewText,
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
        InstrumentFamilyLabelText
    );
    GuiLabel(
        calculateBoundsAtPromptAnchor(context.layout, context.interface, promptBounds.memberLabel),
        InstrumentMemberLabelText
    );
}
