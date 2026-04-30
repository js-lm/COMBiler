#include "note_canvas.hpp"

#include <algorithm>
#include <cmath>

#include <raygui.h>

#include "constants.hpp"
#include "debug_utilities.hpp"

using namespace interface;
namespace canvas_constants = constants::interface_layout::note_canvas;

void NoteCanvas::drawPitchLabels(program_states::InterfaceContext &context){
    auto &state{context.interface.noteCanvas};

    for(int rowIndex{0}; rowIndex < canvas_constants::NumberOfRow; rowIndex++){
        const int semitoneFromC0{semitoneFromRowIndex(rowIndex)};
        const int pitchClass{pitchClassFromSemitone(semitoneFromC0)};

        const bool isNaturalPitchClass{
            pitchClass == 0
         || pitchClass == 2
         || pitchClass == 4
         || pitchClass == 5
         || pitchClass == 7
         || pitchClass == 9
         || pitchClass == 11
        };

        if(state.shouldDrawEveryPitchLabel){
            if(!isNaturalPitchClass) continue;
        }else{
            if(pitchClass != 0) continue;

        }

        const int octaveNumber{semitoneFromC0 / canvas_constants::NumberOfSemitoneInOctave};
        const float snappedRowTopY{rowEdgeY(context, rowIndex)};
        const float snappedRowBottomY{rowEdgeY(context, rowIndex + 1)};
        const Color pitchLabelColor{
            pitchClass == 0 ? state.basePitchLabelColor : state.softerPitchLabelColor
        };

        const int previousTextColorStyleValue{GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)};
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(pitchLabelColor));
        GuiLabel(
            
            Rectangle{
                state.pitchLabelArea.x + canvas_constants::layouts::PitchLabelHorizontalPadding,
                snappedRowTopY,
                state.pitchLabelArea.width - (canvas_constants::layouts::PitchLabelHorizontalPadding * 2.0f),
                snappedRowBottomY - snappedRowTopY
            },
            TextFormat("%s%d", canvas_constants::PitchNames.at(pitchClass), octaveNumber)
        );
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, previousTextColorStyleValue);
        
    }

}

void NoteCanvas::drawBlackKeyRows(program_states::InterfaceContext &context){
    auto &state{context.interface.noteCanvas};

    for(int rowIndex{0}; rowIndex < canvas_constants::NumberOfRow; rowIndex++){
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

void NoteCanvas::drawHorizontalGridLines(program_states::InterfaceContext &context){
    auto &state{context.interface.noteCanvas};

    // DEBUG_PRINT_IF_CHANGED(
    //     "h
    // );

    for(int rowLineIndex{0}; rowLineIndex <= canvas_constants::NumberOfRow; rowLineIndex++){
        const float linePositionY{rowEdgeY(context, rowLineIndex)};
        bool isCBoundary{false};

        if(rowLineIndex > 0 && rowLineIndex < canvas_constants::NumberOfRow){
            const int upperRowIndex{rowLineIndex - 1};
            const int lowerRowIndex{rowLineIndex};

            isCBoundary = isOctaveBoundaryBetweenRows(upperRowIndex, lowerRowIndex);
        }



        DrawLineEx(
            Vector2{state.gridArea.x, linePositionY},
            Vector2{state.gridArea.x + state.gridArea.width, linePositionY},
            canvas_constants::layouts::GridLineThickness,
            isCBoundary ? state.octaveLineColor : state.softGridColor
        );
    }

}

void NoteCanvas::drawVerticalGridLines(program_states::InterfaceContext &context){
    auto &state{context.interface.noteCanvas};

    const float topLinePositionY{rowEdgeY(context, canvas_constants::NumberOfRow)};
    const float bottomLinePositionY{rowEdgeY(context, 0)};


    for(int columnLineIndex{0}; columnLineIndex <= state.activeColumnCount; columnLineIndex++){

        const float linePositionX{
            state.gridArea.x + (static_cast<float>(columnLineIndex) * state.columnWidth)
        };
        const bool isPageBoundary{
            columnLineIndex == 0 || columnLineIndex == state.activeColumnCount
        };
        const bool isMeasureBoundary{
            (columnLineIndex % canvas_constants::layouts::MeasureColumnStep) == 0
        };
        const bool isBeatBoundary{
            (columnLineIndex % canvas_constants::layouts::BeatColumnStep) == 0
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
            Vector2{linePositionX, topLinePositionY},
            Vector2{linePositionX, bottomLinePositionY},
            canvas_constants::layouts::GridLineThickness,
            lineColor
        );

    }



}