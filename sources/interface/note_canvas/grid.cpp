#include "note_canvas.hpp"

#include <algorithm>
#include <cmath>

#include <raygui.h>

#include "constants.hpp"
#include "debug_utilities.hpp"

using namespace interface;
using namespace constants::note_canvas;

void NoteCanvas::drawPitchLabels(program_states::Context &context){
    auto &state{context.interface.noteCanvas};

    for(int rowIndex{0}; rowIndex < NumberOfRow; rowIndex++){
        const int semitoneFromC0{semitoneFromRowIndex(rowIndex)};
        const int pitchClass{pitchClassFromSemitone(semitoneFromC0)};

        if(pitchClass != 0) continue;

        const int octaveNumber{semitoneFromC0 / NumberOfSemitoneInOctave};
        const float snappedRowTopY{rowEdgeY(context, rowIndex)};


        GuiLabel({
			state.pitchLabelArea.x + layouts::PitchLabelHorizontalPadding,
			snappedRowTopY - layouts::PitchLabelVerticalOffset,
			state.pitchLabelArea.width - (layouts::PitchLabelHorizontalPadding * 2.0f),
			layouts::PitchLabelHeight
		},
		TextFormat("%s%d", PitchNames.at(pitchClass), octaveNumber));
        
    }

}

void NoteCanvas::drawBlackKeyRows(program_states::Context &context){
    auto &state{context.interface.noteCanvas};

    for(int rowIndex{0}; rowIndex < NumberOfRow; rowIndex++){
        const int semitoneFromC0{semitoneFromRowIndex(rowIndex)};
        const int pitchClass{pitchClassFromSemitone(semitoneFromC0)};


        if(!isBlackKey(pitchClass)) continue;

        const float rowTopY{rowEdgeY(context, rowIndex)};
        const float rowBottomY{rowEdgeY(context, rowIndex + 1)};

        DrawRectangleRec(
            Rectangle{
                state.gridArea.x,
                rowTopY,
                state.gridArea.width,
                rowBottomY - rowTopY
            },
            state.blackKeyLaneColor
        );


    }
}

void NoteCanvas::drawHorizontalGridLines(program_states::Context &context){
    auto &state{context.interface.noteCanvas};

    // DEBUG_PRINT_IF_CHANGED(
    //     "h
    // );

    for(int rowLineIndex{0}; rowLineIndex <= NumberOfRow; rowLineIndex++){
        const float linePositionY{rowEdgeY(context, rowLineIndex)};
        bool isCBoundary{false};

        if(rowLineIndex > 0 && rowLineIndex < NumberOfRow){
            const int upperRowIndex{rowLineIndex - 1};
            const int lowerRowIndex{rowLineIndex};

            isCBoundary = isOctaveBoundaryBetweenRows(upperRowIndex, lowerRowIndex);
        }



        DrawLineEx(
            Vector2{state.gridArea.x, linePositionY},
            Vector2{state.gridArea.x + state.gridArea.width, linePositionY},
            layouts::GridLineThickness,
            isCBoundary ? state.octaveLineColor : state.softGridColor
        );
    }

}

void NoteCanvas::drawVerticalGridLines(program_states::Context &context){
    auto &state{context.interface.noteCanvas};


    for(int columnLineIndex{0}; columnLineIndex <= state.activeColumnCount; columnLineIndex++){

        const float linePositionX{
            state.gridArea.x + (static_cast<float>(columnLineIndex) * state.columnWidth)
        };
        const bool isPageBoundary{
            columnLineIndex == 0 || columnLineIndex == state.activeColumnCount
        };
        const bool isMeasureBoundary{
            (columnLineIndex % layouts::MeasureColumnStep) == 0
        };
        const bool isBeatBoundary{
            (columnLineIndex % layouts::BeatColumnStep) == 0
        };
        const Color lineColor{
            isPageBoundary
                ? state.octaveLineColor
                : (isMeasureBoundary
                    ? state.strongGridColor
                    : (isBeatBoundary ? state.frameColor : state.softGridColor)
				  )
        };

        DrawLineEx(
            Vector2{linePositionX, state.gridArea.y},
            Vector2{linePositionX, state.gridArea.y + state.gridArea.height},
            layouts::GridLineThickness,
            lineColor
        );

    }



}