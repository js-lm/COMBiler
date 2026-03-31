/*******************************************************************************************
*
*   LayoutName v1.0.0 - Tool Description
*
*   LICENSE: Propietary License
*
*   Copyright (c) 2022 raylib technologies. All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is proprietary and confidential unless the owner allows
*   usage in any other form by expresely written permission.
*
**********************************************************************************************/

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

//----------------------------------------------------------------------------------
// Controls Functions Declaration
//----------------------------------------------------------------------------------
static void MenuButton();                // Button: MenuButton logic
static void NewFileButton();                // Button: NewFileButton logic
static void Button005();                // Button: Button005 logic
static void SaveFileButton();                // Button: SaveFileButton logic
static void ExportFileButton();                // Button: ExportFileButton logic
static void MusicSettingButton();                // Button: MusicSettingButton logic
static void UndoButton();                // Button: UndoButton logic
static void RedoButton();                // Button: RedoButton logic
static void CutNoteButton();                // Button: CutNoteButton logic
static void CopyNoteButton();                // Button: CopyNoteButton logic
static void PasteNoteButton();                // Button: PasteNoteButton logic
static void PreviousPageButton();                // Button: PreviousPageButton logic
static void NextPageButton();                // Button: NextPageButton logic
static void StopButton();                // Button: StopButton logic
static void PlayButton();                // Button: PlayButton logic
static void PageCopyButton();                // Button: PageCopyButton logic
static void PagePasteButton();                // Button: PagePasteButton logic
static void PageCutButton();                // Button: PageCutButton logic

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //---------------------------------------------------------------------------------------
    int screenWidth = 960;
    int screenHeight = 544;

    InitWindow(screenWidth, screenHeight, "layout_name");

    // layout_name: controls initialization
    //----------------------------------------------------------------------------------
    // Const text
    const char *SystemActionsGroupBoxText = nullptr;    // GROUPBOX: SystemActionsGroupBox
    const char *MenuButtonText = "#214#Menu";    // BUTTON: MenuButton
    const char *NewFileButtonText = "#008#";    // BUTTON: NewFileButton
    const char *Button005Text = "#001#";    // BUTTON: Button005
    const char *SaveFileButtonText = "#002#";    // BUTTON: SaveFileButton
    const char *ExportFileButtonText = "#007#";    // BUTTON: ExportFileButton
    const char *StateControlGroupBoxText = nullptr;    // GROUPBOX: StateControlGroupBox
    const char *MusicSettingButtonText = "#140#";    // BUTTON: MusicSettingButton
    const char *UndoButtonText = "#072#";    // BUTTON: UndoButton
    const char *RedoButtonText = "#073#";    // BUTTON: RedoButton
    const char *ActionHistoryDropdownBoxText = "Delete Note";    // DROPDOWNBOX: ActionHistoryDropdownBox
    const char *ToolPaletteGroupBoxText = nullptr;    // GROUPBOX: ToolPaletteGroupBox
    const char *ToolsToggleGroupText = "#021#;#023#;#028#;#124#";    // TOGGLEGROUP: ToolsToggleGroup
    const char *ToolsLabelText = "Tools:";    // LABEL: ToolsLabel
    const char *CutNoteButtonText = "#017#";    // BUTTON: CutNoteButton
    const char *CopyNoteButtonText = "#016#";    // BUTTON: CopyNoteButton
    const char *PasteNoteButtonText = "#018#";    // BUTTON: PasteNoteButton
    const char *ToolOptionsGroupBoxText = nullptr;    // GROUPBOX: ToolOptionsGroupBox
    const char *InspectorGroupBoxText = nullptr;    // GROUPBOX: InspectorGroupBox
    const char *TransportControlsGroupBoxText = nullptr;    // GROUPBOX: TransportControlsGroupBox
    const char *PreviousPageButtonText = "#129#";    // BUTTON: PreviousPageButton
    const char *NextPageButtonText = "#134#";    // BUTTON: NextPageButton
    const char *StopButtonText = "#133#";    // BUTTON: StopButton
    const char *PlayButtonText = "#131#";    // BUTTON: PlayButton
    const char *PageRepeatToggleText = "#061#";    // TOGGLE: PageRepeatToggle
    const char *ChannelListViewText = "All;Instrument 1;Instrument 2;Instrument 3;Chord;Setting";    // LISTVIEW: ChannelListView
    const char *ChannelLabelText = "Channel";    // LABEL: ChannelLabel
    const char *TimelineGroupBoxText = nullptr;    // GROUPBOX: TimelineGroupBox
    const char *TimelineLabelText = "Timeline:";    // LABEL: TimelineLabel
    const char *PageNumberLabelText = "Page: 16/64";    // LABEL: PageNumberLabel
    const char *PageCopyButtonText = "#016#";    // BUTTON: PageCopyButton
    const char *PagePasteButtonText = "#018#";    // BUTTON: PagePasteButton
    const char *PageCutButtonText = "#017#";    // BUTTON: PageCutButton
    const char *PageSelectToggleText = "#109#";    // TOGGLE: PageSelectToggle
    const char *PerPageSettingsGroupBoxText = nullptr;    // GROUPBOX: PerPageSettingsGroupBox
    const char *StatusGroupBoxText = nullptr;    // GROUPBOX: StatusGoupBox
    const char *NoteCanvasGroupBoxText = nullptr;    // GROUPBOX: NoteCanvasGroupBox
    const char *StructureLineText{"Structure"};
    const char *NotePerPageSpinnerText{"Note Per Page:"};
    const char *TempoValueBoxText{"Tempo: "};
    const char *NotesValueBoxText{"Notes: "};
    const char *BarsValueBoxText{"Bars: "};



    // Define anchors
    // toolbar
    Vector2 SystemActionsAnchor = { 0, 0 };            // ANCHOR ID:1
    Vector2 StateControlAnchor = { 248, 0 };            // ANCHOR ID:2
    Vector2 ToolPaletteAnchor = { 448, 0 };            // ANCHOR ID:3
    Vector2 ToolOptionsAnchor = { 784, 0 };            // ANCHOR ID:4

    // sidebar
    Vector2 InspectAnchor = { 0, 40 };            // ANCHOR ID:5

    // navigation bar
    Vector2 TransportControlsAnchor = { 416, 408 };            // ANCHOR ID:6
    Vector2 TimelineAnchor = { 0, 448 };            // ANCHOR ID:7
    Vector2 PerPageSettingsAnchor = { 648, 408 };            // ANCHOR ID:8
    Vector2 StatusAnchor = { 112, 408 };            // ANCHOR ID:9

    // note canvas
    Vector2 NoteCanvasAnchor = { 112, 40 };            // ANCHOR ID:10



    // Define controls variables
    bool ActionHistoryDropdownBoxEditMode = false;
    int ActionHistoryDropdownBoxActive = 0;            // DropdownBox: ActionHistoryDropdownBox
    int ToolsToggleGroupActive = 0;            // ToggleGroup: ToolsToggleGroup
    bool PageRepeatToggleActive = true;            // Toggle: PageRepeatToggle
    int ChannelListViewScrollIndex = 0;
    int ChannelListViewActive = 0;            // ListView: ChannelListView
    Rectangle TimelineScrollPanelScrollView = { 0, 0, 0, 0 };
    Vector2 TimelineScrollPanelScrollOffset = { 0, 0 };
    Vector2 TimelineScrollPanelBoundsOffset = { 0, 0 };            // ScrollPanel: TimelineScrollPanel
    bool PageSelectToggleActive = true;            // Toggle: PageSelectToggle
    bool NotePerPageSpinnerEditMode = false;
    int NotePerPageSpinnerValue = 0;            // Spinner: NotePerPageSpinner
    bool TempoValueBoxEditMode = false;
    int TempoValueBoxValue = 0;            // ValueBOx: TempoValueBox
    bool NotesValueBoxEditMode = false;
    int NotesValueBoxValue = 0;            // ValueBOx: NotesValueBox
    bool BarsValueBoxEditMode = false;
    int BarsValueBoxValue = 0;            // ValueBOx: BarsValueBox

    // Define controls rectangles
    Rectangle layoutRecs[43] = {
        (Rectangle){ SystemActionsAnchor.x + 0, SystemActionsAnchor.y + 0, 248, 40 },    // GroupBox: SystemActionsGroupBox
        (Rectangle){ SystemActionsAnchor.x + 16, SystemActionsAnchor.y + 8, 56, 24 },    // Button: MenuButton
        (Rectangle){ SystemActionsAnchor.x + 86, SystemActionsAnchor.y + 8, 24, 24 },    // Button: NewFileButton
        (Rectangle){ SystemActionsAnchor.x + 114, SystemActionsAnchor.y + 8, 24, 24 },    // Button: Button005
        (Rectangle){ SystemActionsAnchor.x + 142, SystemActionsAnchor.y + 8, 24, 24 },    // Button: SaveFileButton
        (Rectangle){ SystemActionsAnchor.x + 170, SystemActionsAnchor.y + 8, 24, 24 },    // Button: ExportFileButton
        (Rectangle){ StateControlAnchor.x + 0, StateControlAnchor.y + 0, 200, 40 },    // GroupBox: StateControlGroupBox
        (Rectangle){ SystemActionsAnchor.x + 208, SystemActionsAnchor.y + 8, 24, 24 },    // Button: MusicSettingButton
        (Rectangle){ StateControlAnchor.x + 16, StateControlAnchor.y + 8, 24, 24 },    // Button: UndoButton
        (Rectangle){ StateControlAnchor.x + 44, StateControlAnchor.y + 8, 24, 24 },    // Button: RedoButton
        (Rectangle){ StateControlAnchor.x + 80, StateControlAnchor.y + 8, 104, 24 },    // DropdownBox: ActionHistoryDropdownBox
        (Rectangle){ ToolPaletteAnchor.x + 0, ToolPaletteAnchor.y + 0, 336, 40 },    // GroupBox: ToolPaletteGroupBox
        (Rectangle){ ToolPaletteAnchor.x + 56, ToolPaletteAnchor.y + 8, 40, 24 },    // ToggleGroup: ToolsToggleGroup
        (Rectangle){ ToolPaletteAnchor.x + 16, ToolPaletteAnchor.y + 8, 40, 24 },    // Label: ToolsLabel
        (Rectangle){ ToolPaletteAnchor.x + 240, ToolPaletteAnchor.y + 8, 24, 24 },    // Button: CutNoteButton
        (Rectangle){ ToolPaletteAnchor.x + 268, ToolPaletteAnchor.y + 8, 24, 24 },    // Button: CopyNoteButton
        (Rectangle){ ToolPaletteAnchor.x + 296, ToolPaletteAnchor.y + 8, 24, 24 },    // Button: PasteNoteButton
        (Rectangle){ ToolOptionsAnchor.x + 0, ToolOptionsAnchor.y + 0, 176, 40 },    // GroupBox: ToolOptionsGroupBox


        
        (Rectangle){ InspectAnchor.x + 0, InspectAnchor.y + 0, 112, 408 },    // GroupBox: InspectorGroupBox
        (Rectangle){ TransportControlsAnchor.x + 0, TransportControlsAnchor.y + 0, 232, 40 },    // GroupBox: TransportControlsGroupBox
        (Rectangle){ TransportControlsAnchor.x + 32, TransportControlsAnchor.y + 8, 24, 24 },    // Button: PreviousPageButton
        (Rectangle){ TransportControlsAnchor.x + 60, TransportControlsAnchor.y + 8, 24, 24 },    // Button: NextPageButton
        (Rectangle){ TransportControlsAnchor.x + 130, TransportControlsAnchor.y + 8, 24, 24 },    // Button: StopButton
        (Rectangle){ TransportControlsAnchor.x + 102, TransportControlsAnchor.y + 8, 24, 24 },    // Button: PlayButton
        (Rectangle){ TransportControlsAnchor.x + 176, TransportControlsAnchor.y + 8, 24, 24 },    // Toggle: PageRepeatToggle
        (Rectangle){ InspectAnchor.x + 8, InspectAnchor.y + 32, 96, 224 },    // ListView: ChannelListView
        (Rectangle){ InspectAnchor.x + 32, InspectAnchor.y + 8, 48, 24 },    // Label: ChannelLabel
        (Rectangle){ InspectAnchor.x + 8, InspectAnchor.y + 264, 96, 24 },    // Line: StrcutureLine
        (Rectangle){ TimelineAnchor.x + 0, TimelineAnchor.y + 0, 960, 96 },    // GroupBox: TimelineGroupBox
        (Rectangle){ TimelineAnchor.x + 112, TimelineAnchor.y + 24, 776, 72 },    // ScrollPanel: TimelineScrollPanel
        (Rectangle){ TimelineAnchor.x + 112, TimelineAnchor.y + 0, 120, 24 },    // Label: TimelineLabel
        (Rectangle){ TimelineAnchor.x + 816, TimelineAnchor.y + 0, 72, 24 },    // Label: PageNumberLabel
        (Rectangle){ TimelineAnchor.x + 896, TimelineAnchor.y + 24, 24, 24 },    // Button: PageCopyButton
        (Rectangle){ TimelineAnchor.x + 928, TimelineAnchor.y + 24, 24, 24 },    // Button: PagePasteButton
        (Rectangle){ TimelineAnchor.x + 928, TimelineAnchor.y + 56, 24, 24 },    // Button: PageCutButton
        (Rectangle){ TimelineAnchor.x + 896, TimelineAnchor.y + 56, 24, 24 },    // Toggle: PageSelectToggle
        (Rectangle){ PerPageSettingsAnchor.x + 0, PerPageSettingsAnchor.y + 0, 312, 40 },    // GroupBox: PerPageSettingsGroupBox
        (Rectangle){ PerPageSettingsAnchor.x + 208, PerPageSettingsAnchor.y + 8, 88, 24 },    // Spinner: NotePerPageSpinner
        (Rectangle){ StatusAnchor.x + 0, StatusAnchor.y + 0, 304, 40 },    // GroupBox: StatusGoupBox
        (Rectangle){ InspectAnchor.x + 48, InspectAnchor.y + 288, 56, 24 },    // ValueBOx: TempoValueBox
        (Rectangle){ InspectAnchor.x + 48, InspectAnchor.y + 320, 56, 24 },    // ValueBOx: NotesValueBox
        (Rectangle){ InspectAnchor.x + 48, InspectAnchor.y + 352, 56, 24 },    // ValueBOx: BarsValueBox
        (Rectangle){ NoteCanvasAnchor.x + 0, NoteCanvasAnchor.y + 0, 848, 368 },    // GroupBox: NoteCanvasGroupBox
    };
    //----------------------------------------------------------------------------------

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Implement required update logic
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR))); 

            // raygui: controls drawing
            //----------------------------------------------------------------------------------
            // Draw controls
            if (ActionHistoryDropdownBoxEditMode) GuiLock();

            GuiGroupBox(layoutRecs[0], SystemActionsGroupBoxText);
            if (GuiButton(layoutRecs[1], MenuButtonText)) MenuButton(); 
            if (GuiButton(layoutRecs[2], NewFileButtonText)) NewFileButton(); 
            if (GuiButton(layoutRecs[3], Button005Text)) Button005(); 
            if (GuiButton(layoutRecs[4], SaveFileButtonText)) SaveFileButton(); 
            if (GuiButton(layoutRecs[5], ExportFileButtonText)) ExportFileButton(); 
            GuiGroupBox(layoutRecs[6], StateControlGroupBoxText);
            if (GuiButton(layoutRecs[7], MusicSettingButtonText)) MusicSettingButton(); 
            if (GuiButton(layoutRecs[8], UndoButtonText)) UndoButton(); 
            if (GuiButton(layoutRecs[9], RedoButtonText)) RedoButton(); 
            GuiGroupBox(layoutRecs[11], ToolPaletteGroupBoxText);
            GuiToggleGroup(layoutRecs[12], ToolsToggleGroupText, &ToolsToggleGroupActive);
            GuiLabel(layoutRecs[13], ToolsLabelText);
            if (GuiButton(layoutRecs[14], CutNoteButtonText)) CutNoteButton(); 
            if (GuiButton(layoutRecs[15], CopyNoteButtonText)) CopyNoteButton(); 
            if (GuiButton(layoutRecs[16], PasteNoteButtonText)) PasteNoteButton(); 
            GuiGroupBox(layoutRecs[17], ToolOptionsGroupBoxText);
            GuiGroupBox(layoutRecs[18], InspectorGroupBoxText);
            GuiGroupBox(layoutRecs[19], TransportControlsGroupBoxText);
            if (GuiButton(layoutRecs[20], PreviousPageButtonText)) PreviousPageButton(); 
            if (GuiButton(layoutRecs[21], NextPageButtonText)) NextPageButton(); 
            if (GuiButton(layoutRecs[22], StopButtonText)) StopButton(); 
            if (GuiButton(layoutRecs[23], PlayButtonText)) PlayButton(); 
            GuiToggle(layoutRecs[24], PageRepeatToggleText, &PageRepeatToggleActive);
            GuiListView(layoutRecs[25], ChannelListViewText, &ChannelListViewScrollIndex, &ChannelListViewActive);
            GuiLabel(layoutRecs[26], ChannelLabelText);
            GuiLine(layoutRecs[27], StructureLineText);
            GuiGroupBox(layoutRecs[28], TimelineGroupBoxText);
            GuiScrollPanel((Rectangle){layoutRecs[29].x, layoutRecs[29].y, layoutRecs[29].width - TimelineScrollPanelBoundsOffset.x, layoutRecs[29].height - TimelineScrollPanelBoundsOffset.y }, nullptr, layoutRecs[29], &TimelineScrollPanelScrollOffset, &TimelineScrollPanelScrollView);
            GuiLabel(layoutRecs[30], TimelineLabelText);
            GuiLabel(layoutRecs[31], PageNumberLabelText);
            if (GuiButton(layoutRecs[32], PageCopyButtonText)) PageCopyButton(); 
            if (GuiButton(layoutRecs[33], PagePasteButtonText)) PagePasteButton(); 
            if (GuiButton(layoutRecs[34], PageCutButtonText)) PageCutButton(); 
            GuiToggle(layoutRecs[35], PageSelectToggleText, &PageSelectToggleActive);
            GuiGroupBox(layoutRecs[36], PerPageSettingsGroupBoxText);
            if (GuiSpinner(layoutRecs[37], NotePerPageSpinnerText, &NotePerPageSpinnerValue, 0, 100, NotePerPageSpinnerEditMode)) NotePerPageSpinnerEditMode = !NotePerPageSpinnerEditMode;
            GuiGroupBox(layoutRecs[38], StatusGroupBoxText);
            if (GuiValueBox(layoutRecs[39], TempoValueBoxText, &TempoValueBoxValue, 0, 100, TempoValueBoxEditMode)) TempoValueBoxEditMode = !TempoValueBoxEditMode;
            if (GuiValueBox(layoutRecs[40], NotesValueBoxText, &NotesValueBoxValue, 0, 100, NotesValueBoxEditMode)) NotesValueBoxEditMode = !NotesValueBoxEditMode;
            if (GuiValueBox(layoutRecs[41], BarsValueBoxText, &BarsValueBoxValue, 0, 100, BarsValueBoxEditMode)) BarsValueBoxEditMode = !BarsValueBoxEditMode;
            GuiGroupBox(layoutRecs[42], NoteCanvasGroupBoxText);
            if (GuiDropdownBox(layoutRecs[10], ActionHistoryDropdownBoxText, &ActionHistoryDropdownBoxActive, ActionHistoryDropdownBoxEditMode)) ActionHistoryDropdownBoxEditMode = !ActionHistoryDropdownBoxEditMode;
            
            GuiUnlock();
            //----------------------------------------------------------------------------------

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------------
// Controls Functions Definitions (local)
//------------------------------------------------------------------------------------
// Button: MenuButton logic
static void MenuButton()
{
    // TODO: Implement control logic
}
// Button: NewFileButton logic
static void NewFileButton()
{
    // TODO: Implement control logic
}
// Button: Button005 logic
static void Button005()
{
    // TODO: Implement control logic
}
// Button: SaveFileButton logic
static void SaveFileButton()
{
    // TODO: Implement control logic
}
// Button: ExportFileButton logic
static void ExportFileButton()
{
    // TODO: Implement control logic
}
// Button: MusicSettingButton logic
static void MusicSettingButton()
{
    // TODO: Implement control logic
}
// Button: UndoButton logic
static void UndoButton()
{
    // TODO: Implement control logic
}
// Button: RedoButton logic
static void RedoButton()
{
    // TODO: Implement control logic
}
// Button: CutNoteButton logic
static void CutNoteButton()
{
    // TODO: Implement control logic
}
// Button: CopyNoteButton logic
static void CopyNoteButton()
{
    // TODO: Implement control logic
}
// Button: PasteNoteButton logic
static void PasteNoteButton()
{
    // TODO: Implement control logic
}
// Button: PreviousPageButton logic
static void PreviousPageButton()
{
    // TODO: Implement control logic
}
// Button: NextPageButton logic
static void NextPageButton()
{
    // TODO: Implement control logic
}
// Button: StopButton logic
static void StopButton()
{
    // TODO: Implement control logic
}
// Button: PlayButton logic
static void PlayButton()
{
    // TODO: Implement control logic
}
// Button: PageCopyButton logic
static void PageCopyButton()
{
    // TODO: Implement control logic
}
// Button: PagePasteButton logic
static void PagePasteButton()
{
    // TODO: Implement control logic
}
// Button: PageCutButton logic
static void PageCutButton()
{
    // TODO: Implement control logic
}

