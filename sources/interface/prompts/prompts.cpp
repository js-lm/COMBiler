#include "prompts.hpp"

#include <raygui.h>

#include <algorithm>
#include <cmath>
#include <string>

#include <fmt/core.h>

#include "constants.hpp"
#include "constants/serializer.hpp"

#include "labels.hpp"

#include "interface/utilities.hpp"

#include "utilities/project_utilities.hpp"

#include "external/raygui/raygui_wrapper.h"

#include "debug_utilities.hpp"

#include <magic_enum/magic_enum.hpp>

using namespace interface;
namespace prompts_constants = constants::labels::prompts;

void Prompts::draw(program_states::InterfaceContext &context){
    auto &promptState{context.interface.prompts};

    if(!promptState.isAnyPromptVisible()) return;

    const Rectangle overlayBounds{
        0, 0,
        static_cast<float>(context.system.window.interfaceRenderTextureWidth),
        static_cast<float>(context.system.window.interfaceRenderTextureHeight)
    };
    DrawRectangleRec(overlayBounds, Fade(BLACK, constants::prompts::OverlayDimAlpha));

    if(promptState.isMusicSettingPromptVisible){
        drawMusicSettingPrompt(context);
        return;
    }

    if(promptState.isConstantsManagerWindowVisible){
        drawConstantsManagerPrompt(context);
        return;
    }

    if(promptState.isInfoWindowVisible){
        drawInfoPrompt(context);
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

void Prompts::drawInfoPrompt(program_states::InterfaceContext &context){
    auto &promptState{context.interface.prompts};

    const auto anchor{context.layout.anchor.prompts.infoWindow};
    const auto &bounds{context.layout.bounds.prompts.infoWindow};

    if(GuiWindowBox(calculateBoundsAtAnchor(anchor, bounds.windowBox), prompts_constants::AboutInfoWindowBoxText)){
        promptState.isInfoWindowVisible = false;
        return;
    }

// #ifdef PLATFORM_WEB
//     const std::string windowTitle{std::string{constants::application_window::Title} + constants::application_window::WebEditionSuffix};
//     GuiLabel(calculateBoundsAtAnchor(anchor, bounds.titleLabel), windowTitle.c_str());
// #else
    GuiLabel(calculateBoundsAtAnchor(anchor, bounds.titleLabel), constants::application_window::Title);
// #endif
    GuiLine(calculateBoundsAtAnchor(anchor, bounds.infoLine), nullptr);
    
    if(GuiLabelButton(calculateBoundsAtAnchor(anchor, bounds.repoLink1Button), prompts_constants::AboutHardwareRepoLabelText)){
        OpenURL(prompts_constants::AboutHardwareRepoURL);
    }
    if(GuiLabelButton(calculateBoundsAtAnchor(anchor, bounds.repoLink2Button), prompts_constants::AboutSoftwareRepoLabelText)){
        OpenURL(prompts_constants::AboutSoftwareRepoURL);
    }

    GuiLabel(calculateBoundsAtAnchor(anchor, bounds.descriptionLabel), prompts_constants::AboutDescriptionLabelText);
    GuiLabel(calculateBoundsAtAnchor(anchor, bounds.poweredByLabel), prompts_constants::AboutPoweredByLabelText);
    GuiLabel(calculateBoundsAtAnchor(anchor, bounds.librariesLabel), prompts_constants::AboutLibrariesLabelText);

    const int previousLabelAlignment{GuiGetStyle(LABEL, TEXT_ALIGNMENT)};
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_RIGHT);
    GuiLabel(calculateBoundsAtAnchor(anchor, bounds.timestampLabel), fmt::format(prompts_constants::AboutTimestampFormatText, __DATE__, __TIME__).c_str());
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, previousLabelAlignment);

    const int previousStatusBarAlignment{GuiGetStyle(STATUSBAR, TEXT_ALIGNMENT)};
    GuiSetStyle(STATUSBAR, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiStatusBar(calculateBoundsAtAnchor(anchor, bounds.copyrightedBar), prompts_constants::AboutCopyrightedBarText);
    GuiSetStyle(STATUSBAR, TEXT_ALIGNMENT, previousStatusBarAlignment);
}

void Prompts::drawMusicSettingPrompt(program_states::InterfaceContext &context){
    auto &promptState{context.interface.prompts};
    const auto anchor{context.layout.anchor.prompts.musicSettingWindow};
    const auto &bounds{context.layout.bounds.prompts.musicSetting};
    const auto projectData{utilities::projectDataFrom(context.system)};

    if(!projectData) return;

    if(GuiWindowBox(calculateBoundsAtAnchor(anchor, bounds.windowBox), prompts_constants::MusicSettingWindowBoxText)){
        promptState.isMusicSettingPromptVisible = false;
        return;
    }

    GuiLabel(calculateBoundsAtAnchor(anchor, bounds.titleLabel), prompts_constants::MusicSettingTitleLabelText);
    if(GuiTextBoxFPSIndependent(
        calculateBoundsAtAnchor(anchor, bounds.titleTextBox),
        projectData->metadata.title,
        constants::project_data::MetadataTitleMaximumLength + 1,
        promptState.musicSettingTitleTextBoxEditMode
    )){
        promptState.musicSettingTitleTextBoxEditMode = !promptState.musicSettingTitleTextBoxEditMode;
        if(!promptState.musicSettingTitleTextBoxEditMode){
            DEBUG_PRINT("Updated Project Title: {}", projectData->metadata.title);
            promptState.hasModifiedMetadata = true;
        }
    }

    GuiLabel(calculateBoundsAtAnchor(anchor, bounds.authorLabel), prompts_constants::MusicSettingAuthorLabelText);
    if(GuiTextBoxFPSIndependent(
        calculateBoundsAtAnchor(anchor, bounds.authorTextBox),
        projectData->metadata.author,
        constants::project_data::MetadataAuthorMaximumLength + 1,
        promptState.musicSettingAuthorTextBoxEditMode
    )){
        promptState.musicSettingAuthorTextBoxEditMode = !promptState.musicSettingAuthorTextBoxEditMode;
        if(!promptState.musicSettingAuthorTextBoxEditMode){
            DEBUG_PRINT("Updated Project Author: {}", projectData->metadata.author);
            promptState.hasModifiedMetadata = true;
        }
    }

    GuiLabel(calculateBoundsAtAnchor(anchor, bounds.createdLabel), prompts_constants::MusicSettingCreatedLabelText);
    const std::string creationDateString{(projectData->metadata.creationDate[0] == '\0') ? prompts_constants::MusicSettingNotSavedYetText : projectData->metadata.creationDate};
    GuiLabel(calculateBoundsAtAnchor(anchor, bounds.createdDateLabel), creationDateString.c_str());

    GuiLabel(calculateBoundsAtAnchor(anchor, bounds.modifiedLabel), prompts_constants::MusicSettingModifiedLabelText);
    const std::string modificationDateString{(projectData->metadata.modificationDate[0] == '\0') ? prompts_constants::MusicSettingNotSavedYetText : projectData->metadata.modificationDate};
    GuiLabel(calculateBoundsAtAnchor(anchor, bounds.modifiedDateLabel), modificationDateString.c_str());

    GuiLabel(calculateBoundsAtAnchor(anchor, bounds.versionLabel), prompts_constants::MusicSettingVersionLabelText);
    const std::string versionString{fmt::format("{} ({})", constants::serializer::Version, COMBILER_VERSION)};
    GuiLabel(calculateBoundsAtAnchor(anchor, bounds.versionDateLabel), versionString.c_str());

    if(GuiCheckBox(calculateBoundsAtAnchor(anchor, bounds.readOnlyCheckBox), prompts_constants::MusicSettingReadOnlyCheckBoxText, &projectData->metadata.isReadOnly)){
        promptState.hasModifiedMetadata = true;
    }

    GuiLine(calculateBoundsAtAnchor(anchor, bounds.line), prompts_constants::MusicSettingLineText);

    GuiLabel(calculateBoundsAtAnchor(anchor, bounds.tempoLabel), prompts_constants::MusicSettingNotesPerSecondLabelText);
    
    int maximumTempoValue{static_cast<int>(projectData->metadata.maximumTempo)};
    if(GuiSpinner(
        calculateBoundsAtAnchor(anchor, bounds.tempoValueBox),
        nullptr,
        &maximumTempoValue,
        1, 100,
        promptState.musicSettingMaximumTempoValueBoxEditMode
    )){
        promptState.musicSettingMaximumTempoValueBoxEditMode = !promptState.musicSettingMaximumTempoValueBoxEditMode;
        if(!promptState.musicSettingMaximumTempoValueBoxEditMode){
            DEBUG_PRINT("Updated Maximum Tempo: {}", maximumTempoValue);
            promptState.hasModifiedMetadata = true;
        }
    }
    projectData->metadata.maximumTempo = static_cast<float>(maximumTempoValue);

    GuiLabel(calculateBoundsAtAnchor(anchor, bounds.notesLabel), prompts_constants::MusicSettingNotesPerPageLabelText);
    
    const int previousNotesValue{projectData->metadata.notePerPage};
    if(GuiSpinner(
        calculateBoundsAtAnchor(anchor, bounds.notesValueBox),
        nullptr,
        &projectData->metadata.notePerPage,
        constants::project_data::MinimumNotePerPage,
        constants::project_data::MaximumNotePerPage,
        promptState.musicSettingNotesValueBoxEditMode
    )){
        promptState.musicSettingNotesValueBoxEditMode = !promptState.musicSettingNotesValueBoxEditMode;
        if(!promptState.musicSettingNotesValueBoxEditMode){
            DEBUG_PRINT("Updated Notes Per Page: {}", projectData->metadata.notePerPage);
            promptState.hasModifiedMetadata = true;
        }
    }

    if(projectData->metadata.notePerPage != previousNotesValue){
        projectData->metadata.notePerPage = std::clamp(
            projectData->metadata.notePerPage,
            constants::project_data::MinimumNotePerPage,
            constants::project_data::MaximumNotePerPage
        );
        const int currentPageNoteCount{utilities::currentPageNoteCountFrom(*projectData, context.system.project.currentPage)};
        context.interface.navigationBar.notePerPageSpinnerValue = currentPageNoteCount;
        context.interface.noteCanvas.isGridLayoutDirty = true;
    }
}
