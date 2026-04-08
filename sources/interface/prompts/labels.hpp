#pragma once

namespace constants::labels::prompts{

    constexpr const char *TempoWindowBoxText{nullptr};
    constexpr const char *TempoGroupBoxText{"Tempo"};
    constexpr const char *TempoSliderText{"Playback Speed"};
    constexpr const char *TempoPercentageLabelText{"%"};
    constexpr const char *TempoDescriptionLabelText{"Sets playback speed.  2% increments."};
    constexpr const char *ConfirmButtonText{"#112#Confirm"};
    constexpr const char *BackButtonText{"#113#Back"};

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

} // namespace constants::labels::prompts
