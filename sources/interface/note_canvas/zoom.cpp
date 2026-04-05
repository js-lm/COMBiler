#include "note_canvas.hpp"

#include <raygui.h>

#include "labels.hpp"

#include "interface/utilities.hpp"

#include "debug_utilities.hpp"

#include "constants.hpp"

#include <algorithm>
#include <cmath>

using namespace interface;
using namespace constants::interface_layout::note_canvas;

void NoteCanvas::drawScrollBar(program_states::InterfaceContext &context){
	auto &state{context.interface.noteCanvas};

	if(!state.isVerticalScrollBarVisible) return;

	Vector2 scrollOffset{0, -state.verticalScrollInPixels};
	Rectangle viewBounds{};

	GuiScrollPanel(
		state.verticalScrollBarBounds,
		nullptr,
		state.verticalScrollBarContentBounds,
		&scrollOffset,
		&viewBounds
	);

	state.verticalScrollInPixels = std::clamp(
		-scrollOffset.y,
		.0f,
		state.maximumVerticalScrollInPixels
	);

}

void NoteCanvas::handleZoom(program_states::InterfaceContext &context){
	// const auto anchor{context.layout.anchor.noteCanvas.noteCanvas};
	auto &state{context.interface.noteCanvas};

	// if(!CheckCollisionPointRec(GetMousePosition(), calculateBoundsAtAnchor(anchor, state.drawableArea))) return;

	const float mouseWheelMovement{GetMouseWheelMove()};
	if(mouseWheelMovement == .0f) return;

	const float previousVerticalZoomFactor{state.verticalZoomFactor};
	const bool isControlPressed{IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)};

	if(!isControlPressed){
		const float updatedVerticalScrollInPixels{
			state.verticalScrollInPixels - (mouseWheelMovement * zoom::MouseWheelPixelStep)
		};
		state.verticalScrollInPixels = std::clamp(
			updatedVerticalScrollInPixels,
			.0f,
			state.maximumVerticalScrollInPixels
		);
		return;
	}

	const float updatedVerticalZoomFactor{
		std::clamp(
			previousVerticalZoomFactor + (mouseWheelMovement * zoom::MouseWheelStep),
			zoom::MinimumVerticalFactor,
			state.maximumAllowedVerticalZoomFactor
		)
	};

	if(updatedVerticalZoomFactor == previousVerticalZoomFactor) return;

    
	const float cursorPositionInViewportY{GetMouseY() - state.gridAreaTopScreenPositionY};

	const float previousTotalHeightInPixels{state.gridArea.height * previousVerticalZoomFactor};
	const float updatedTotalHeightInPixels{state.gridArea.height * updatedVerticalZoomFactor};

	const float cursorVirtualPositionBeforeZoomInPixels{
		state.verticalScrollInPixels + cursorPositionInViewportY
	};
	const float cursorVirtualRatioInGrid{
		cursorVirtualPositionBeforeZoomInPixels / previousTotalHeightInPixels
	};


	const float updatedVerticalScrollInPixels{
		(cursorVirtualRatioInGrid * updatedTotalHeightInPixels) - cursorPositionInViewportY
	};

	state.verticalZoomFactor    = updatedVerticalZoomFactor;
	state.verticalScrollInPixels = std::clamp(
		updatedVerticalScrollInPixels,
		.0f,
		updatedTotalHeightInPixels - state.gridArea.height
	);
	state.isGridLayoutDirty = true;
}
