#pragma once

#include <raylib.h>

struct SystemState{

    struct Toolbar{
        bool isMenuButtonPressed{false};
        bool isNewFileButtonPressed{false};
        bool isButton005Pressed{false};
        bool isSaveFileButtonPressed{false};
        bool isExportFileButtonPressed{false};

        bool isMusicSettingButtonPressed{false};

        bool isUndoButtonPressed{false};
        bool isRedoButtonPressed{false};

        bool actionHistoryDropdownEditMode{false};
        int actionHistoryDropdownActive{0};

        int selectedToolIndex{0};

        bool isCutNoteButtonPressed{false};
        bool isCopyNoteButtonPressed{false};
        bool isPasteNoteButtonPressed{false};
    } toolbar;

    struct Sidebar{



    } sidebar;

    struct NavigationBar{



    } navigationBar;

    struct NoteCanvas{



    } noteCanvas;

};