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

public:
    Enumerator() = delete;

    static std::string print(
        const program_states::ProjectData   &projectData, 
        const units::enumerator::Paper      paper,
        const units::enumerator::Mm         margin
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
    static std::string toPdfColor(const units::enumerator::Color);

private:
    static int calculateStripsPerSheet(        
        const units::enumerator::Paper  paper,
        const units::enumerator::Mm     margin
    );

    static int calculateRowsPerStripSegment(        
        const units::enumerator::Paper  paper,
        const units::enumerator::Mm     margin
    );
};