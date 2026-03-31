#pragma once

#include <raylib.h>

#include "interface/anchors.hpp"

using namespace constants::anchors::toolbar;

namespace constants::layout::toolbar{

    constexpr Rectangle SystemActionsGroupBox{SystemActionsAnchor.x + 0, SystemActionsAnchor.y + 0, 248, 40};
    constexpr Rectangle MenuButton{SystemActionsAnchor.x + 16, SystemActionsAnchor.y + 8, 56, 24};
    constexpr Rectangle NewFileButton{SystemActionsAnchor.x + 86, SystemActionsAnchor.y + 8, 24, 24};
    constexpr Rectangle LoadFileButton{SystemActionsAnchor.x + 114, SystemActionsAnchor.y + 8, 24, 24};
    constexpr Rectangle SaveFileButton{SystemActionsAnchor.x + 142, SystemActionsAnchor.y + 8, 24, 24};
    constexpr Rectangle ExportFileButton{SystemActionsAnchor.x + 170, SystemActionsAnchor.y + 8, 24, 24};

    constexpr Rectangle StateControlGroupBox{StateControlAnchor.x + 0, StateControlAnchor.y + 0, 200, 40};
    constexpr Rectangle MusicSettingButton{SystemActionsAnchor.x + 208, SystemActionsAnchor.y + 8, 24, 24};
    constexpr Rectangle UndoButton{StateControlAnchor.x + 16, StateControlAnchor.y + 8, 24, 24};
    constexpr Rectangle RedoButton{StateControlAnchor.x + 44, StateControlAnchor.y + 8, 24, 24};
    constexpr Rectangle ActionHistoryDropdownBox{StateControlAnchor.x + 80, StateControlAnchor.y + 8, 104, 24};

    constexpr Rectangle ToolPaletteGroupBox{ToolPaletteAnchor.x + 0, ToolPaletteAnchor.y + 0, 336, 40};
    constexpr Rectangle ToolsToggleGroup{ToolPaletteAnchor.x + 56, ToolPaletteAnchor.y + 8, 40, 24};
    constexpr Rectangle ToolsLabel{ToolPaletteAnchor.x + 16, ToolPaletteAnchor.y + 8, 40, 24};
    constexpr Rectangle CutNoteButton{ToolPaletteAnchor.x + 240, ToolPaletteAnchor.y + 8, 24, 24};
    constexpr Rectangle CopyNoteButton{ToolPaletteAnchor.x + 268, ToolPaletteAnchor.y + 8, 24, 24};
    constexpr Rectangle PasteNoteButton{ToolPaletteAnchor.x + 296, ToolPaletteAnchor.y + 8, 24, 24};

    constexpr Rectangle ToolOptionsGroupBox{ToolOptionsAnchor.x + 0, ToolOptionsAnchor.y + 0, 176, 40};


} // constants::anchors