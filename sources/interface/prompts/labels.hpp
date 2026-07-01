#pragma once

namespace constants::labels::prompts{

    constexpr const char *TempoWindowBoxText{nullptr};
    constexpr const char *TempoGroupBoxText{"Tempo"};
    constexpr const char *TempoSliderText{"Playback Speed"};
    constexpr const char *TempoPercentageLabelText{"%"};
    constexpr const char *TempoDescriptionLabelText{"Sets playback speed.  2% increments."};
    constexpr const char *ConfirmButtonText{"#112#Confirm"};
    constexpr const char *BackButtonText{"#113#Back"};

    constexpr const char *FileDropWarningWindowBoxText{"Warning"};
    constexpr const char *OverwriteWarningText{"Are you sure you want to overwrite your project?"};
    constexpr const char *YesButtonText{"#112#Yes"};
    constexpr const char *NoButtonText{"#113#No"};

    constexpr const char *VolumeWindowBoxText{nullptr};
    constexpr const char *VolumeGroupBoxText{"Volume"};
    constexpr const char *VolumeLevelToggleGroupText{"1;2;3;4;5;6;7;8;9;10"};
    constexpr const char *VolumeDescriptionLabelText{"Sets target volume.  10% increments."};
    constexpr const char *VolumeTargetGroupBoxText{"Target"};
    constexpr const char *VolumeTargetLabelText{"Channel:"};
    constexpr const char *VolumeTargetToggleGroupText{"All;1;2;3;4"};

    constexpr const char *ArticulationWindowBoxText{nullptr};
    constexpr const char *ArticulationGroupBoxText{"Articulation"};
    constexpr const char *ArticulationStateToggleGroupText{"#032#Staccato;#033#Normal;#034#Legato;#035#Sustain;#036#Latch"};
    constexpr const char *ArticulationStateLabelText{"Staccato\nNormal\nLegato\nSustain\nLatch"};
    constexpr const char *ArticulationDescriptionLabelText{"Release at 1/2 step\nRelease at 1 step\nRelease upon No-op or a new note\nRelease upon a new note\nRelease when the identical note is triggered again"};
    constexpr const char *ArticulationTargetGroupBoxText{"Channel"};
    constexpr const char *ArticulationTargetToggleGroupText{"All Channels;Channel 1;Channel 2;Channel 3;Channel 4"};

    constexpr const char *InstrumentFamilyListViewText{
        "Keyboard;Percussion;Woodwind;String;Brass;Harmony;Guitar;Bass;Reed;Synth;World;Drum Sets"
    };

    constexpr const char *InstrumentWindowBoxText{"Select an instrument"};
    constexpr const char *InstrumentFamilyLabelText{"Family"};
    constexpr const char *InstrumentMemberLabelText{"Member"};

    // constants manager
    constexpr const char *ConstantsManagerWindowBoxText{"Constants Manager"};
    constexpr const char *ConstantsManagerWindowBoxUnsavedText{"Constants Manager - Unsaved*"};
    constexpr const char *EditNameGroupBoxText{"Edit Constant"};
    constexpr const char *TextConstantLabelText{"Name:"};
    constexpr const char *TypeToggleGroupText{"None;#030#;#029#;#028#"};

    constexpr const char *IDBarText{"ID"};
    constexpr const char *NameBarText{"Name"};
    constexpr const char *TypeBarText{"Type"};
    constexpr const char *ValueBarText{"Value"};

    constexpr const char *DefaultNameLabel{"--"};
    constexpr const char *DefaultTypeLabel{"None"};
    constexpr const char *DefaultValueLabel{""};

    constexpr const char *TempoTypeLabel{"#030#Tempo"};
    constexpr const char *VolumeTypeLabel{"#029#Volume"};
    constexpr const char *ArticulationTypeLabel{"#028#Articul."};

    constexpr const char *ConstantsManagerTempoGroupBoxText{"Tempo"};
    constexpr const char *TargetGroupBoxText{"Target"};
    constexpr const char *TargetToggleGroupText{"All;1;2;3;4"};
    constexpr const char *TargetChannelLabelText{"Channel:"};
    constexpr const char *ConstantsManagerTempoSliderText{"Playback:"};

    constexpr const char *ConstantsManagerCommitButtonText{"#112#Commit"};
    constexpr const char *ConstantsManagerCommitButtonUnsavedText{"#112#Commit*"};
    constexpr const char *ConstantsManagerRevertButtonText{"#113#Revert"};

    constexpr const char *SwapGroupBoxText{"Swap"};
    constexpr const char *SwapButtonText{"Swap"};
    constexpr const char *SwapLeftLabelText{"Left:"};
    constexpr const char *SwapRightLabelText{"Right:"};

    constexpr const char *ConstantsManagerArticulationGroupBoxText{"Articulation"};
    constexpr const char *ConstantsManagerArticulationToggleGroupText{"#032#;#033#;#034#;#035#;#036#"};

    constexpr const char *ConstantsManagerVolumeGroupBoxText{"Volume"};
    constexpr const char *ConstantsManagerVolumeToggleGroupText{"1;2;3;4;5;6;7;8;9;10"};
    constexpr const char *VolumeLabelText{"Volume:"};

    constexpr const char *LoadButtonText{"Load"};
    
    constexpr const char *WarningWindowBoxText{"Unsaved changes"};
    constexpr const char *WarningLabelText{"You haven't committed your changes. Commit now?"};
    constexpr const char *WarningYesButtonText{"Yes"};
    constexpr const char *WarningNoButtonText{"No"};

    constexpr const char *InfoWindowBoxText{"Undefined constant"};
    constexpr const char *InfoLabelText{"The constant is not defined"};
    constexpr const char *InfoOKButtonText{"OK"};

    constexpr const char *AboutInfoWindowBoxText{"#043#About COMBiler"};
    constexpr const char *AboutHardwareRepoLabelText{"Hardware Repo: github.com/js-lm/COMB-Optical-Music-Box"};
    constexpr const char *AboutHardwareRepoURL{"https://github.com/js-lm/COMB-Optical-Music-Box"};
    constexpr const char *AboutSoftwareRepoLabelText{"Software Repo: github.com/js-lm/COMBiler"};
    constexpr const char *AboutSoftwareRepoURL{"https://github.com/js-lm/COMBiler"};
    constexpr const char *AboutDescriptionLabelText{"A music \"compiler\" for COMB Optical Music Box"};
    constexpr const char *AboutPoweredByLabelText{"Powered by:"};
    constexpr const char *AboutLibrariesLabelText{"raylib, raygui, TinySoundFont, tinyfiledialog, magic_enum, & fmt."};
    constexpr const char *AboutTimestampFormatText{"Compiled on {} @ {}"};
    constexpr const char *AboutCopyrightedBarText{"Copyright (C) 2026. Released under the GNU GPL v3."};

} // namespace constants::labels::prompts
