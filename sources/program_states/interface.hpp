#pragma once

#include <raylib.h>

#include <optional>
#include <array>
#include <vector>
#include <cstring>

#include "aliases.hpp"
#include "constants.hpp"

#include "command/command_type.hpp"

namespace program_states{

    struct Interface{
        enum class SelectionDomain{
            Notes,
            Pages,
        };

        SelectionDomain activeSelectionDomain{SelectionDomain::Notes};

        struct Toolbar{
            bool isMenuButtonPressed        {false};
            bool isNewFileButtonPressed     {false};
            bool isLoadFileButtonPressed    {false};
            bool isSaveFileButtonPressed    {false};
            bool isSaveAsFileButtonPressed  {false};
            bool isExportFileButtonPressed  {false};

            bool isMusicSettingButtonPressed{false};

            bool isUndoButtonPressed        {false};
            bool isRedoButtonPressed        {false};
            bool isConstantsButtonPressed   {false};

            constants::toolbar::Tool selectedTool{constants::toolbar::Tool::Cursor};

            bool isCutNoteButtonPressed     {false};
            bool isCopyNoteButtonPressed    {false};
            bool isPasteNoteButtonPressed   {false};
            bool isInfoButtonPressed        {false};
        } toolbar{};

        struct Prompts{
            constants::prompts::CommandPrompt selectedCommandTool{constants::prompts::CommandPrompt::Tempo};

            bool isCommandWindowVisible     {false};
            constants::prompts::CommandPrompt activeCommandPrompt{constants::prompts::CommandPrompt::Tempo};

            int instrumentFamilyListViewScrollIndex{0};
            int instrumentFamilyListViewIndex{0};
            int instrumentMemberListViewScrollIndex{0};
            int instrumentMemberListViewIndex{0};

            music_data::InstrumentFamily selectedInstrumentFamily{music_data::InstrumentFamily::Keyboard};
            music_data::Instrument selectedInstrument{music_data::Instrument::Grand_Piano};

            float tempoPlaybackSpeedPercentage{80.0f};
            bool tempoPercentageValueBoxEditMode{false};
            int tempoPercentageValue        {0};

            int volumeLevelToggleIndex      {7};
            int volumeTargetToggleIndex     {0};

            int articulationStateToggleIndex{1};
            int articulationTargetToggleIndex{0};

            struct ConstantsManagerDraft{
                int constantIndex;
                std::optional<constants::prompts::constants_manager::Type> commandType;

                char nameTextBoxText[constants::prompts::constants_manager::ConstantNameTextMaximumLength];

                float tempoSliderValue;

                int targetToggleGroupIndex;
                int articulationToggleGroupIndex;
                int volumeToggleGroupIndex;

                char swapLeftTextBoxText[constants::prompts::constants_manager::SwapIndexTextMaximumLength];
                char swapRightTextBoxText[constants::prompts::constants_manager::SwapIndexTextMaximumLength];

            public:
                ConstantsManagerDraft(){ reset();}

                void reset(bool shouldResetConstantIndex = true){
                    if(shouldResetConstantIndex) constantIndex = 0;
                
                    commandType.reset();

                    strcpy(nameTextBoxText, "Untitled Constant");

                    tempoSliderValue = 80.0f;

                    targetToggleGroupIndex = 0;
                    articulationToggleGroupIndex = 1;
                    volumeToggleGroupIndex = 8;

                    strcpy(swapLeftTextBoxText, "00");
                    strcpy(swapRightTextBoxText, "01");
                }
            } draft{};
            
            bool hasModified{false};
            int requestedConstantIndex{0};

            bool hasRequestedCommit{false};

            bool isRightPanelDirty{false};

            command::ConstantIndex loadedConstantIndex{0};

            bool isConstantsManagerWindowVisible{false};
            bool isConstantsManagerWarningWindowVisible{false};
            bool isConstantsManagerInfoWindowVisible{false};

            Rectangle constantsManagerScrollPanelScrollView{0, 0, 0, 0};
            Vector2 constantsManagerScrollPanelScrollOffset{0, 0};
            Vector2 constantsManagerScrollPanelBoundsOffset{0, 0};

            bool constantsManagerNameTextBoxEditMode{false};
            
            int constantsManagerTypeToggleGroupIndex{constants::prompts::constants_manager::toIndex(constants::prompts::constants_manager::Type::None)};


            bool constantsManagerSwapLeftTextBoxEditMode{false};
            bool constantsManagerSwapRightTextBoxEditMode{false};

        } prompts{};

        struct Sidebar{
            int selectedChannelListViewScrollIndex{0};
            int selectedChannelListViewIndex{0};

            bool tempoValueBoxEditMode  {false};
            int tempoValue              {0};

            bool notesValueBoxEditMode  {false};
            int notesValue              {32};
            bool isShowCommandsEnabled  {false};

        } sidebar{};

        struct NavigationBar{
            // bool isPlaying{false};

            bool isPreviousPageButtonPressed{false};
            bool isNextPageButtonPressed    {false};

            bool isPageCopyButtonPressed    {false};
            bool isPagePasteButtonPressed   {false};
            bool isPageCutButtonPressed     {false};

            bool isPageRepeatEnabled        {false};
            bool isPageSelectEnabled        {false};

            bool notePerPageSpinnerEditMode{false};
            int notePerPageSpinnerValue     {32};
            int requestedPageNumber         {0};
            bool isAddPageRequested         {false};
            int requestedPageInsertionIndex {constants::action_center::InvalidPageInsertionIndex};
            bool isPageMoveRequested        {false};
            int requestedPageMoveSourceIndex{constants::action_center::InvalidPageInsertionIndex};
            int requestedPageMoveInsertionIndex{constants::action_center::InvalidPageInsertionIndex};

            bool isTimelineDragCandidate    {false};
            bool isTimelineDraggingPage     {false};
            int timelineDragCandidatePageIndex{-1};
            int timelineDraggedPageIndex    {-1};
            int timelineDropInsertionIndex  {0};
            double timelineDragCandidateStartTimeInSeconds{.0};
            Vector2 timelineDragStartMouseScreenPosition{0, 0};
            float timelineDragGrabOffsetInPixelsX{.0f};

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
            Rectangle verticalScrollBarBounds{0, 0, 1, 1};
            Rectangle verticalScrollBarContentBounds{0, 0, 1, 1};

            float scissorAnchorY        {.0f};
            float columnWidth           {1.0f};
            float verticalZoomFactor    {1.0f};
            float verticalScrollInPixels{.0f};
            float maximumVerticalScrollInPixels{.0f};
            // float maximumVerticalScrollPanelOffsetInPixels{.0f};
            float maximumAllowedVerticalZoomFactor{1.0f};
            float gridAreaTopScreenPositionY{.0f};
            float rowHeightInPixels{1.0f};
            bool shouldDrawEveryPitchLabel{true};

            bool isVerticalScrollBarVisible{false};

            Color frameColor            {BLANK};
            Color strongGridColor       {BLANK};
            Color softGridColor         {BLANK};
            Color octaveLineColor       {BLANK};
            Color blackKeyLaneColor     {BLANK};
            Color pitchLabelBackgroundColor{BLANK};
            Color basePitchLabelColor   {BLANK};
            Color softerPitchLabelColor {BLANK};

            bool isGridLayoutDirty{true};

            // std::optional<int> pressedPianoKeyIndex{};

            struct CursorPosition{
                int noteIndex{0};
                music_data::Note note{music_data::Note::C0};

                // TODO: isHoveringNote has some frame delay, not that it affect anything, but still kinda want to fix it
                bool isHoveringNote{false};
            };
            std::optional<CursorPosition> cursorPosition{};


        } noteCanvas{};

        struct Clipboard{
            // using SelectionArea = Rectangle;

            struct SelectionArea{
                int topLeftColumnIndex{0};
                int topLeftRowIndex{0};
                int widthInCells{0};
                int heightInCells{0};
            };

            std::optional<SelectionArea> selectionArea{};

            bool hasCopiedSelection{false};
            bool isPasteModeEnabled{false};

            int copiedWidthInCells{0};
            int copiedHeightInCells{0};
            int copiedCenterRowIndex{0};
            bool copiedFromAllChannels{false};
            int copiedInstrumentChannelIndex{constants::interface_layout::note_canvas::notes::NoInstrumentChannelIndex};
            
            bool hasPasteAnchor{false};
            int pasteAnchorNoteIndex{0};
            int pasteAnchorCenterRowIndex{0};

            std::array<
                std::vector<std::optional<music_data::InstrumentChannelData>>,
                constants::project_data::NumberOfInstrumentChannels
            > instrumentChannels{};

            std::vector<std::optional<command::CommandToken>> commandChannel{};
        } clipboard{};

    };

} // namespace program_states