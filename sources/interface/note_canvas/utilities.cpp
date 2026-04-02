#include "note_canvas.hpp"

#include <algorithm>

#include <cmath>

#include <raygui.h>

#include "constants.hpp"
#include "debug_utilities.hpp"

using namespace interface;
using namespace constants::note_canvas;

bool NoteCanvas::isBlackKey(int pitchClass){
    return std::find(
        BlackKeyIndices.begin(),
        BlackKeyIndices.end(),
        pitchClass
    ) != BlackKeyIndices.end();
}

float NoteCanvas::rowEdgeY(program_states::Context &context, int rowLineIndex){
    auto &state{context.interface.noteCanvas};
    return state.gridArea.y + static_cast<float>((rowLineIndex * state.totalHeightInPixels) / NumberOfRow);
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

    state.pitchLabelArea = {
        state.drawableArea.x,
        state.drawableArea.y,
        std::min(static_cast<float>(layouts::PitchLabelColumnWidth), state.drawableArea.width),
        state.drawableArea.height
    };

    state.gridArea = {
        state.pitchLabelArea.x + state.pitchLabelArea.width,
        state.drawableArea.y,
        state.drawableArea.width - state.pitchLabelArea.width,
        state.drawableArea.height
    };

    state.activeColumnCount 	= context.interface.navigationBar.notePerPageSpinnerValue;
    state.totalHeightInPixels 	= static_cast<int>(std::round(state.gridArea.height));
    state.columnWidth 			= state.gridArea.width / static_cast<float>(state.activeColumnCount);

    state.frameColor 			= GetColor(GuiGetStyle(LISTVIEW, BORDER_COLOR_NORMAL));
    state.strongGridColor 		= Fade(state.frameColor, layouts::StrongGridLineAlpha);
    state.softGridColor 		= Fade(state.frameColor, layouts::SoftGridLineAlpha);
    state.octaveLineColor 		= Fade(state.frameColor, layouts::OctaveGridLineAlpha);
    state.blackKeyLaneColor 	= Fade(state.frameColor, layouts::BlackKeyLaneAlpha);
    state.pitchLabelBackgroundColor = Fade(state.frameColor, layouts::PitchLabelBackgroundAlpha);

    state.scissorAnchorY = state.drawableArea.y + context.system.noteCanvas.gridCamera.offset.y;

    state.scissorBounds = {
        std::floor(state.drawableArea.x + context.system.noteCanvas.gridCamera.offset.x) - layouts::ScissorExtraPadding,
        std::floor(state.scissorAnchorY - context.system.noteCanvas.gridCamera.target.y) - layouts::ScissorExtraPadding,
        std::ceil(state.drawableArea.width) + layouts::ScissorExtraPadding * 2,
        std::ceil(state.drawableArea.height) + layouts::ScissorExtraPadding * 2
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