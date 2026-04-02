#include "note_canvas.hpp"

#include <algorithm>
#include <iostream>
#include <cmath>

#include <raygui.h>

#include "constants.hpp"
#include "debug_utilities.hpp"

using namespace interface;

void NoteCanvas::drawGrid(program_states::Context &context){
	auto &state{context.interface.noteCanvas};

	if(state.isGridLayoutDirty) cleanGridLayout(context);

	const auto &camera{context.system.noteCanvas.gridCamera};
	const Rectangle translatedPitchLabelArea{
		state.pitchLabelArea.x + camera.target.x,
		state.pitchLabelArea.y,
		state.pitchLabelArea.width,
		state.pitchLabelArea.height
	};


	BeginScissorMode(
		state.scissorBounds.x,
		state.scissorBounds.y,
		state.scissorBounds.width,
		state.scissorBounds.height
	);

	BeginMode2D(camera); {
		context.interface.noteCanvas.pressedPianoKeyIndex = std::nullopt;

		DrawRectangleRec(translatedPitchLabelArea, state.pitchLabelBackgroundColor);

		drawBlackKeyRows(context);
		drawPitchLabels(context);
		drawHorizontalGridLines(context);
		drawVerticalGridLines(context);


		DrawLineEx(
			Vector2{
				translatedPitchLabelArea.x + translatedPitchLabelArea.width,
				translatedPitchLabelArea.y
			},
			Vector2{
				translatedPitchLabelArea.x + translatedPitchLabelArea.width,
				translatedPitchLabelArea.y + translatedPitchLabelArea.height
			},
			constants::note_canvas::layouts::GridLineThickness,
			state.octaveLineColor
		);

	} EndMode2D();
	EndScissorMode();
	
}