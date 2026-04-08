#pragma once

#include <array>

#include <raylib.h>

#include "aliases.hpp"

namespace constants::interface_layout::note_canvas{

    constexpr int NumberOfRow{75};
    constexpr int FirstNoteOffsetFromC0{11}; // B0

    constexpr int NumberOfSemitoneInOctave{12};

    constexpr int IconSize{16};
    constexpr int InstrumentIconPadding{1};

    namespace layouts{

        constexpr int LeftPadding           {1};
        constexpr int RightPadding          {12};
        constexpr int TopPadding            {20};
        constexpr int BottomPadding         {20};
        constexpr int PitchLabelColumnWidth {42};

        constexpr int ScissorExtraPadding{2};

        constexpr int GridLineThickness {1};
        constexpr int BeatColumnStep    {4};
        constexpr int MeasureColumnStep {8};

        constexpr float StrongGridLineAlpha     {.50f};
        constexpr float SoftGridLineAlpha       {.25f};
        constexpr float OctaveGridLineAlpha     {.65f};
        constexpr float BlackKeyLaneAlpha       {.14f};
        constexpr float PitchLabelBackgroundAlpha{.12f};
        constexpr float NonCPitchLabelAlpha     {.55f};

        constexpr int PitchLabelHorizontalPadding   {3};
        constexpr int PitchLabelVerticalOffset      {3};
        constexpr int PitchLabelHeight              {11};
        constexpr float FullPitchLabelMinimumRowHeight{14.0f};

    } // namespace layouts

    constexpr std::array<int, 5> BlackKeyIndices{1, 3, 6, 8, 10};

    constexpr std::array<const char*, 12> PitchNames{
        "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
    };

    namespace zoom{
        constexpr float MinimumVerticalFactor{1.0f};
        constexpr float MaximumVerticalFactor{8.0f};
        constexpr float MouseWheelStep       {.5f};

        constexpr float MouseWheelPixelStep{100.0f};
    } // namespace scroll

    namespace notes{
        constexpr float BorderThicknessInPixels{1.0f};

        constexpr int AllChannelsListViewIndex{0};
        constexpr int SystemChannelListViewIndex{5};

        constexpr float SelectedChannelAlpha    {1.0f};
        constexpr float UnselectedChannelAlpha  {.4f};
        constexpr float AllChannelsAlpha        {.8f};

        // TODO: use enum map
        constexpr std::array<Color, 4> ChannelNoteColors{
            Color{250, 176, 5, 255},
            Color{51, 154, 240, 255},
            Color{32, 201, 151, 255},
            Color{190, 75, 219, 255}
        };

        constexpr Color NoteBorderColor{0, 0, 0, 255};
        constexpr int InstrumentIndexMarkerRowIndex{0};
        constexpr float InstrumentIndexColorDarkeningFactor{.35f};
        constexpr Color InstrumentIndexIconColor{245, 245, 245, 255};
    } // namespace notes

    namespace commands{
        constexpr Color TempoColor{145, 145, 145, 255};
        constexpr Color VolumeColor{78, 180, 115, 255};
        constexpr Color ArticulationColor{232, 145, 53, 255};

        constexpr const char *TempoText{"Tempo"};
        constexpr const char *VolumeText{"Volume"};
        constexpr const char *ArticulationText{"Articulation"};
        constexpr const char *EmptyText{""};
        constexpr const char *UnsignedIntegerFormatText{"%d"};

        constexpr int TempoIconIndex{30};
        constexpr int VolumeIconIndex{29};

        constexpr std::array<int, 5> ArticulationIconIndices{32, 33, 34, 35, 36};
        constexpr std::array<const char*, 5> ArticulationNames{
            "Staccato",
            "Normal",
            "Legato",
            "Sustain",
            "Infinite"
        };

        constexpr std::array<const char*, 5> TargetFullNames{
            "All",
            "Channel 1",
            "Channel 2",
            "Channel 3",
            "Channel 4"
        };

        constexpr std::array<const char*, 5> TargetCompactNames{
            "All", "Ch1", "Ch2", "Ch3", "Ch4"
        };

        constexpr float FullVisibilityAlpha{.95f};
        constexpr float TransparentVisibilityAlpha{.6f};

        constexpr float HorizontalPadding{3.0f};
        constexpr int TextFontSize{10};
        constexpr float LineHalfHeight{5.0f};

        // TODO: they should be relative to the screen instead
        constexpr float ConstantStringCenterRatio{.24f};
        constexpr float IconCenterRatio{.42f};
        constexpr float NameCenterRatio{.5f};
        constexpr float ValueCenterRatio{.66f};
        constexpr float TargetCenterRatio{.82f};

        constexpr float InstrumentVerticalIconCenterRatio{.3f};
        constexpr float InstrumentVerticalTextTopPaddingInPixels{8.0f};

        inline constexpr int articulationIconIndex(command::Articulation::Type articulationType){
            return ArticulationIconIndices[static_cast<size_t>(articulationType)];
        }

        inline constexpr const char *articulationName(command::Articulation::Type articulationType){
            return ArticulationNames[static_cast<size_t>(articulationType)];
        }

        inline constexpr const char *targetFullName(command::Target target){
            return TargetFullNames[static_cast<size_t>(target)];
        }

        inline constexpr const char *targetCompactName(command::Target target){
            return TargetCompactNames[static_cast<size_t>(target)];
        }


    } // namespace commands

} // namespace constants::note_canvas
