#pragma once

#include "program_states/context.hpp"

namespace interface{

    class NoteCanvas{
    public:
        static void draw(program_states::InterfaceContext &context);

    private:
        static void drawGrid(program_states::InterfaceContext &context);

        static void drawPitchLabels(program_states::InterfaceContext &context);
        static void drawBlackKeyRows(program_states::InterfaceContext &context);

        static void drawHorizontalGridLines(program_states::InterfaceContext &context);
        static void drawVerticalGridLines(program_states::InterfaceContext &context);

        static void drawScrollBar(program_states::InterfaceContext &context);

        static void drawNotes(program_states::InterfaceContext &context);

    private:
        static void handleZoom(program_states::InterfaceContext &context);

    private:
        static bool isBlackKey(int pitchClass);
        static float rowEdgeY(program_states::InterfaceContext &context, int rowLineIndex);

        static int semitoneFromRowIndex(int rowIndex);
        static int pitchClassFromSemitone(int semitoneFromC0);
        static bool isOctaveBoundaryBetweenRows(int upperRowIndex, int lowerRowIndex);

        static void cleanGridLayout(program_states::InterfaceContext &context);

        static float maximumVerticalZoomFactorForSquareCell(program_states::InterfaceContext &context);

        static music_data::Instrument instrumentAtPageStart(
            const program_states::ProjectData &projectData, 
            size_t instrumentChannelIndex, 
            int currentPageIndex
        );
    };

} // namespace interface
