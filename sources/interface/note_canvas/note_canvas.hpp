#pragma once

#include <string>

#include "program_states/context.hpp"

namespace interface{

    class NoteCanvas{
    public:
        static void draw(program_states::InterfaceContext &context);

    private:
        struct BigNote{
            Color baseColor{BLANK};
            int iconIndex{0};
            std::string firstTextLine{};
            std::string secondTextLine{};
            std::string thirdTextLine{};
            bool shouldDrawFirstTextLineVertically{false};
        };

    private:
        static void drawGrid(program_states::InterfaceContext &context);

        static void drawPitchLabels(program_states::InterfaceContext &context);
        static void drawBlackKeyRows(program_states::InterfaceContext &context);

        static void drawHorizontalGridLines(program_states::InterfaceContext &context);
        static void drawVerticalGridLines(program_states::InterfaceContext &context);

        static void drawScrollBar(program_states::InterfaceContext &context);

        static void drawNotes(program_states::InterfaceContext &context);

        static void drawCommands(program_states::InterfaceContext &context);


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

    private:
        static std::optional<command::Command> commandFromToken(const command::CommandToken &token);
        static bool doesTextFitWidth(const std::string &text, float width, int fontSize);
        static std::string trimTextToFitWidth(const std::string &text, float width, int fontSize);
        static std::string targetTextForWidth(command::Target target, float width, int fontSize);

        static BigNote createCommandBigNote(const command::Command &commandData, float width, int fontSize);
        static BigNote createInstrumentBigNote(Color baseColor, music_data::Instrument instrument);

        static void drawCenteredTextLine(const Rectangle &columnBounds, float centerY, const std::string &text, Color textColor);
        static void drawBigNote(
            program_states::InterfaceContext &context,
            const Rectangle &columnBounds,
            const BigNote &bigNote,
            float alpha,
            bool shouldCenterIconInColumn
        );
    };

} // namespace interface
