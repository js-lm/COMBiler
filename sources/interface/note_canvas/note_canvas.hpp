#pragma once

#include "program_states/context.hpp"

namespace interface{

    class NoteCanvas{
    public:
        static void draw(program_states::Context &context);

    private:
        static void drawGrid(program_states::Context &context);
        static void drawPitchLabels(program_states::Context &context);
        static void drawBlackKeyRows(program_states::Context &context);
        static void drawHorizontalGridLines(program_states::Context &context);
        static void drawVerticalGridLines(program_states::Context &context);
        static void drawScrollBar(program_states::Context &context);

    private:
        static void handleZoom(program_states::Context &context);

    private:
        static bool isBlackKey(int pitchClass);
        static float rowEdgeY(program_states::Context &context, int rowLineIndex);

        static int semitoneFromRowIndex(int rowIndex);
        static int pitchClassFromSemitone(int semitoneFromC0);
        static bool isOctaveBoundaryBetweenRows(int upperRowIndex, int lowerRowIndex);

        static void cleanGridLayout(program_states::Context &context);

        static float maximumVerticalZoomFactorForSquareCell(program_states::Context &context);
    };

} // namespace interface
