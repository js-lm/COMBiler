#pragma once

#include <raylib.h>

#include <optional>

namespace program_states{

    struct Interface{

        struct Toolbar{
            bool isMenuButtonPressed        {false};
            bool isNewFileButtonPressed     {false};
            bool isLoadFileButtonPressed    {false};
            bool isSaveFileButtonPressed    {false};
            bool isExportFileButtonPressed  {false};

            bool isMusicSettingButtonPressed{false};

            bool isUndoButtonPressed        {false};
            bool isRedoButtonPressed        {false};

            bool actionHistoryDropdownEditMode{false};
            int actionHistoryDropdownActive {0};

            int selectedToolIndex           {0};

            bool isCutNoteButtonPressed     {false};
            bool isCopyNoteButtonPressed    {false};
            bool isPasteNoteButtonPressed   {false};
        } toolbar{};

        struct Sidebar{
            int selectedChannelListViewScrollIndex{0};
            int selectedChannelListViewIndex{0};

            bool tempoValueBoxEditMode  {false};
            int tempoValue              {0};

            bool notesValueBoxEditMode  {false};
            int notesValue              {0};

            bool barsValueBoxEditMode   {false};
            int barsValue               {0};

        } sidebar{};

        struct NavigationBar{
            bool isPreviousPageButtonPressed{false};
            bool isNextPageButtonPressed    {false};
            bool isPlayButtonPressed        {false};
            bool isStopButtonPressed        {false};

            bool isPageCopyButtonPressed    {false};
            bool isPagePasteButtonPressed   {false};
            bool isPageCutButtonPressed     {false};

            bool isPageRepeatEnabled        {true};
            bool isPageSelectEnabled        {true};

            bool notePerPageSpinnerEditMode{false};
            int notePerPageSpinnerValue     {32};

            Rectangle timelineScrollPanelScrollView{0, 0, 0, 0};
            Vector2 timelineScrollPanelScrollOffset{0, 0};
            Vector2 timelineScrollPanelBoundsOffset{0, 0};

        } navigationBar{};

        struct NoteCanvas{
            Rectangle drawableArea      {0, 0, 0, 0};
            Rectangle gridArea          {0, 0, 0, 0};
            Rectangle pianoKeyboardArea {0, 0, 0, 0};
            Rectangle pitchLabelArea    {0, 0, 0, 0};

            int activeColumnCount{32};
            int totalHeightInPixels{1};

            Rectangle scissorBounds{0, 0, 1, 1};

            float scissorAnchorY{.0f};
            float columnWidth   {1.0f};

            Color frameColor        {0, 0, 0, 255};
            Color strongGridColor   {0, 0, 0, 255};
            Color softGridColor     {0, 0, 0, 255};
            Color octaveLineColor   {0, 0, 0, 255};
            Color blackKeyLaneColor {0, 0, 0, 255};
            Color pitchLabelBackgroundColor{0, 0, 0, 255};

            bool isGridLayoutDirty{true};

            std::optional<int> pressedPianoKeyIndex{};

        } noteCanvas{};

    };

} // namespace program_states