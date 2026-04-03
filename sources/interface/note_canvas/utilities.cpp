#include "note_canvas.hpp"

#include <algorithm>

#include <cmath>

#include <raygui.h>

#include "constants.hpp"
#include "debug_utilities.hpp"

using namespace interface;
using namespace constants::interface_layout::note_canvas;

bool NoteCanvas::isBlackKey(int pitchClass){
    return std::find(
        BlackKeyIndices.begin(),
        BlackKeyIndices.end(),
        pitchClass
    ) != BlackKeyIndices.end();
}

float NoteCanvas::rowEdgeY(program_states::Context &context, int rowLineIndex){
    auto &state{context.interface.noteCanvas};
    return state.gridArea.y - state.verticalScrollInPixels + static_cast<float>((rowLineIndex * state.totalHeightInPixels) / NumberOfRow);
}

int NoteCanvas::semitoneFromRowIndex(int rowIndex){
    return FirstNoteOffsetFromC0 + (NumberOfRow - 1 - rowIndex);
}

int NoteCanvas::pitchClassFromSemitone(int semitoneFromC0){
    return semitoneFromC0 % NumberOfSemitoneInOctave;
}

bool NoteCanvas::isOctaveBoundaryBetweenRows(int upperRowIndex, int lowerRowIndex){
    const int upperSemitoneFromC0{semitoneFromRowIndex(upperRowIndex)};
    const int lowerSemitoneFromC0{semitoneFromRowIndex(lowerRowIndex)};

    const int upperPitchClass{pitchClassFromSemitone(upperSemitoneFromC0)};
    const int lowerPitchClass{pitchClassFromSemitone(lowerSemitoneFromC0)};

    return upperPitchClass == 0 && lowerPitchClass == (NumberOfSemitoneInOctave - 1);
}

void NoteCanvas::cleanGridLayout(program_states::Context &context){
	const auto &bounds{context.layout.bounds.noteCanvas};
	const auto &groupBox{bounds.groupBox};
    const auto &anchor{context.layout.anchor.noteCanvas.noteCanvas};

	auto &state{context.interface.noteCanvas};

    state.drawableArea = {
        layouts::LeftPadding,
        layouts::TopPadding,
        groupBox.width - layouts::LeftPadding - layouts::RightPadding,
        groupBox.height - layouts::TopPadding - layouts::BottomPadding
    };

    // DEBUG_PRINT_IF_CHANGED("{},{},{},{}",
    //     state.drawableArea.x,
    //     state.drawableArea.y,
    //     state.drawableArea.width,
    //     state.drawableArea.height
    // );

    state.pitchLabelArea = {0, 0, layouts::PitchLabelColumnWidth, groupBox.height};

    state.gridArea = {
        state.pitchLabelArea.x + state.pitchLabelArea.width,
        state.drawableArea.y,
        state.drawableArea.width - state.pitchLabelArea.width,
        state.drawableArea.height
    };

    state.activeColumnCount 	= context.interface.navigationBar.notePerPageSpinnerValue;
    const float maximumAllowedVerticalZoomFactor{maximumVerticalZoomFactorForSquareCell(context)};
    state.maximumAllowedVerticalZoomFactor = maximumAllowedVerticalZoomFactor;
    state.verticalZoomFactor = std::clamp(
        state.verticalZoomFactor,
        zoom::MinimumVerticalFactor,
        maximumAllowedVerticalZoomFactor
    );
    state.totalHeightInPixels 	= static_cast<int>(std::round(state.gridArea.height * state.verticalZoomFactor));
    state.columnWidth 			= state.gridArea.width / static_cast<float>(state.activeColumnCount);
    state.rowHeightInPixels     = static_cast<float>(state.totalHeightInPixels) / static_cast<float>(NumberOfRow);
    state.shouldDrawEveryPitchLabel = state.rowHeightInPixels >= layouts::FullPitchLabelMinimumRowHeight;

    state.maximumVerticalScrollInPixels = static_cast<float>(state.totalHeightInPixels) - state.gridArea.height;
    state.verticalScrollInPixels = std::clamp(
        state.verticalScrollInPixels,
        .0f,
        state.maximumVerticalScrollInPixels
    );
    state.isVerticalScrollBarVisible = state.maximumVerticalScrollInPixels > .0f;

    const float defaultBorderWidth{static_cast<float>(GuiGetStyle(DEFAULT, BORDER_WIDTH))};
    const float listViewScrollBarWidth{static_cast<float>(GuiGetStyle(LISTVIEW, SCROLLBAR_WIDTH))};
    const float scrollPanelWidth{listViewScrollBarWidth + (defaultBorderWidth * 2.0f)};
    const float groupBoxToGridHeightOffset{groupBox.height - state.gridArea.height};
    state.verticalScrollBarBounds = {
        anchor.x + groupBox.width - scrollPanelWidth,
        anchor.y,
        scrollPanelWidth,
        groupBox.height
    };
    state.verticalScrollBarContentBounds = {
        0, 0, 0,
        static_cast<float>(state.totalHeightInPixels) + groupBoxToGridHeightOffset - defaultBorderWidth
    };

    state.frameColor 			= GetColor(GuiGetStyle(LISTVIEW, BORDER_COLOR_NORMAL));
    state.strongGridColor 		= Fade(state.frameColor, layouts::StrongGridLineAlpha);
    state.softGridColor 		= Fade(state.frameColor, layouts::SoftGridLineAlpha);
    state.octaveLineColor 		= Fade(state.frameColor, layouts::OctaveGridLineAlpha);
    state.blackKeyLaneColor 	= Fade(state.frameColor, layouts::BlackKeyLaneAlpha);
    state.pitchLabelBackgroundColor = Fade(state.frameColor, layouts::PitchLabelBackgroundAlpha);
    state.basePitchLabelColor   = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));
    state.softerPitchLabelColor = Fade(state.basePitchLabelColor, layouts::NonCPitchLabelAlpha);

    state.gridAreaTopScreenPositionY = anchor.y + state.gridArea.y;

    state.scissorAnchorY = state.drawableArea.y + context.system.noteCanvas.gridCamera.offset.y;

    state.scissorBounds = {
        context.system.noteCanvas.gridCamera.offset.x,
        context.layout.anchor.noteCanvas.noteCanvas.y + context.system.noteCanvas.gridCamera.target.y,
        groupBox.width,
        groupBox.height
    };

    // DEBUG_PRINT(
    //     "cleanGridLayout activeColumnCount={} drawableArea=({},{},{},{}) gridArea=({},{},{},{}) columnWidth={}",
    //     state.activeColumnCount,
    //     state.drawableArea.x,
    //     state.drawableArea.y,
    //     state.drawableArea.width,
    //     state.drawableArea.height,
    //     state.gridArea.x,
    //     state.gridArea.y,
    //     state.gridArea.width,
    //     state.gridArea.height,
    //     state.columnWidth
    // );

    state.isGridLayoutDirty = false;
}

float NoteCanvas::maximumVerticalZoomFactorForSquareCell(program_states::Context &context){
    auto &state{context.interface.noteCanvas};

    const float columnWidth{
        state.gridArea.width / static_cast<float>(state.activeColumnCount)
    };
    const float squareCellVerticalZoomFactor{
        (columnWidth * static_cast<float>(NumberOfRow)) / state.gridArea.height
    };


    return std::max(
        zoom::MinimumVerticalFactor,
        std::min(zoom::MaximumVerticalFactor, squareCellVerticalZoomFactor)
    );
    
}
