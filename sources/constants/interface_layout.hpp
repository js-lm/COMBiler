#pragma once

namespace constants::interface_layout{

    // TODO: namespace them

    constexpr int NavigationRowVerticalOffset{136};
    constexpr int TimelineVerticalOffset{96};

    constexpr int TimelineScrollPanelRightPadding{72};
    constexpr int TimelinePageNumberLabelOffset{144};
    constexpr int TimelinePageNumberValueLabelOffset{112};
    constexpr int TimelinePageCopyButtonOffset{64};
    constexpr int TimelinePagePasteButtonOffset{32};
    constexpr int TimelinePageCutButtonOffset{32};
    constexpr int TimelinePageSelectToggleOffset{64};

    constexpr int NavigationContentLeftEdgeX{112};
    constexpr int NavigationSectionCount{3};

    constexpr int PerPageSpinnerRightPadding{96};
    constexpr int ToolOptionInfoButtonRightPadding{16};

    constexpr int TransportControlsButtonsTotalWidth{168};
    constexpr int TransportControlsNextPageButtonOffset{28};
    constexpr int TransportControlsPlayButtonOffset{70};
    constexpr int TransportControlsStopButtonOffset{98};
    constexpr int TransportControlsPageRepeatToggleOffset{144};

    namespace timeline{
        constexpr float ContentOriginXInPixels{.0f};
        constexpr float ContentOriginYInPixels{.0f};

        constexpr float BlockHeightInPixels{52.0f};
        constexpr float MinimumBlockWidthInPixels{8.0f};
        constexpr float BlockWidthPerNoteInPixels{2.0f};
        constexpr float BlockSpacingInPixels{8.0f};

        constexpr float InsertionMarkerWidthInPixels{4.0f};
        constexpr float InsertionMarkerHeightInPixels{20.0f};
        constexpr float InsertionMarkerTopOffsetInPixels{16.0f};

        constexpr float PlusButtonSizeInPixels{24.0f};
        constexpr float PlusButtonOffsetInPixels{8.0f};

        constexpr float ContentLeftPaddingInPixels{4.0f};
        constexpr float ContentTopPaddingInPixels{4.0f};
        constexpr float StaticCornerSizeInPixels{14.0f};

        constexpr float DragStartDistanceThresholdInPixels{4.0f};
        constexpr double InsertionDoubleClickThresholdInSeconds{.3};

        constexpr float MarkerHalfWidthScale{.5f};
        constexpr float PlusButtonVerticalCenterScale{.5f};

        constexpr int InvalidIndex{-1};
        constexpr int MinimumIndex{0};
        constexpr int FirstPageNumber{1};

        constexpr float StaticCornerBorderThicknessInPixels{1.0f};
        constexpr float BlockBorderThicknessInPixels{2.0f};
    } // namespace timeline

} // namespace constants::interface_layout