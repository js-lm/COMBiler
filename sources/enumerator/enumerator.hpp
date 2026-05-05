#pragma once

#include "program_states/project_data.hpp"

#include "constants.hpp"
#include "aliases.hpp"

#include <string>
#include <vector>
#include <array>

#include "pdf_canvas.hpp"
#include "pdf_document.hpp"

class Enumerator{
public:
    using EncodedRow = std::array<
        units::enumerator::Color, 
        constants::enumerator::NumberOfColumn
    >;

    using EncodedInstruction = std::array<
        units::enumerator::Color, 
        constants::enumerator::NumberOfDigitPerInstruction
    >;
    // using EncodedCommand = std::array<
    //     units::enumerator::Color, 
    //     constants::enumerator::NumberOfDigitCommand
    // >;

    // using CompiledData = std::vector<EncodedRow>;

    // using PDF = std::vector<std::string>;

// private:
//     int stripPerSheet_{1};
//     int rowsPerStripSegment_{1};

//     units::Mm paperWidth_{216};
//     units::Mm paperHeight_{279};
//     units::Mm paperMargin_{10};

    // std::vector<EncodedRow> compiledData_{};

public:
    Enumerator() = delete;
    // Enumerator() = default;
    // ~Enumerator() = default;

    static void print(
        const program_states::ProjectData   &projectData, 
        const units::enumerator::Paper      paper,
        const units::Mm                     margin
    );

private:
    static std::vector<EncodedRow> compileProjectData(const program_states::ProjectData &projectData);

private:
    static EncodedInstruction encodeCommandData(
        const std::optional<command::CommandToken>  &commandToken,
        const command::CommandPalette               &commandPalette
    );
    static EncodedInstruction encodeInstrumentData(
        const std::optional<music_data::InstrumentChannelData> &channelData
    );

    static Enumerator::EncodedInstruction splitToBase5(int value, int opcodeOffset = 0);

private:
    // void updateStripsPerSheet();
    // void updateRowsPerStripSegment();
    static int calculateStripsPerSheet(
        units::Mm paperHeight,
        units::Mm paperMargin
    );
    static int calculateRowsPerStripSegment(
        units::Mm paperWidth,
        units::Mm paperMargin
    );

private:
    static void generatePaperStrip(
        PdfDocument &pdf, 
        const units::Mm width,
        const units::Mm height,
        const units::Mm margin,
        const std::vector<Enumerator::EncodedRow> &compiledPaperStrip
    );

private:
    static void drawStripSegment(
        PdfCanvas &canvas, 
        units::Mm startX, 
        units::Mm startY, 
        const std::vector<EncodedRow> &compiledPaperStrip, 
        size_t startIndex, 
        size_t count
    );
    static void drawOpticalMarkers(PdfCanvas &canvas, units::Mm x, units::Mm y);
    static void drawBoundaryLine(PdfCanvas &canvas, units::Mm x, units::Mm y);
    static void drawCells(
        PdfCanvas &canvas, 
        units::Mm x,
        units::Mm y,
        const std::vector<EncodedRow> &compiledPaperStrip,
        const size_t currentPaperStripRow
    );
};