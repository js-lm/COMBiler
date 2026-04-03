#pragma once

#include <array>

namespace constants::interface_layout::note_canvas{

    constexpr int NumberOfRow{75};
    constexpr int FirstNoteOffsetFromC0{11}; // B0

    constexpr int NumberOfSemitoneInOctave{12};

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
        constexpr float MouseWheelStep{.5f};

        constexpr float MouseWheelPixelStep{100.0f};
    } // namespace scroll

} // namespace constants::note_canvas
