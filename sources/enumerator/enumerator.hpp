#pragma once

#include "program_states/project_data.hpp"

#include "constants.hpp"
#include "aliases.hpp"

#include <string>
#include <vector>
#include <array>

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

    using PDF = std::vector<std::string>;

private:
    int stripPerSheet_{1};
    int rowsPerStripSegment_{1};

    units::Mm paperWidth_{216};
    units::Mm paperHeight_{279};
    units::Mm paperMargin_{10};

    PDF pdfStream_{};

    std::vector<EncodedRow> compiledData_{};

public:
    Enumerator() = delete;

    PDF print(
        const program_states::ProjectData   &projectData, 
        const units::enumerator::Paper      paper,
        const units::Mm                     margin
    );

private:
    std::vector<EncodedRow> compileProjectData(const program_states::ProjectData &projectData) const;

private:
    EncodedInstruction encodeCommandData(
        const std::optional<command::CommandToken>  &commandToken,
        const command::CommandPalette               &commandPalette
    ) const;
    EncodedInstruction encodeInstrumentData(
        const std::optional<music_data::InstrumentChannelData> &channelData
    ) const;

    Enumerator::EncodedInstruction splitToBase5(int value, int opcodeOffset = 0) const;

private:
    // void updateStripsPerSheet();
    // void updateRowsPerStripSegment();
    int calculateStripsPerSheet(
        units::Mm paperHeight,
        units::Mm paperMargin
    ) const;
    int calculatePerStripSegment(
        units::Mm paperWidth,
        units::Mm paperMargin
    ) const;

private:
    void drawOpticalMarkersToString();
    void drawBoundaryLineToString();
    void drawCellsToString();
    void drawGridLinesToString();

// private:
//     void drawRectangle(units::Mm x, units::Mm y, units::Mm width, units::Mm height, units::enumerator::Color color);
//     void drawRectangleLines(units::Mm x, units::Mm y, units::Mm width, units::Mm height, units::enumerator::Color color);
//     void drawLines(units::Mm startX, units::Mm startY, units::Mm endX, units::Mm endY, units::enumerator::Color color);
//     void draw7SegmentDigit(int digit, units::Mm x, units::Mm y);

private:
    std::string toPdfColor(const units::enumerator::Color color) const;
};