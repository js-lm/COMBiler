#pragma once

#include "aliases.hpp"

namespace constants::enumerator{

    constexpr units::Mm MaximumMargin{50.0f};
    constexpr units::Mm MinimumMargin{.0f};

    constexpr int NumberOfColumn{16}; // by column I mean looking at it vertically
    constexpr int NumberOfDigitPerInstruction{3};

    constexpr units::Mm CellWidth{5.85f};
    constexpr units::Mm CellHeight{5.84f};
    constexpr units::Mm OpticalMarkHeight{11.0f};
    constexpr units::Mm CellStrokeWidth{.1f};
    constexpr units::Mm BottomLineOffset{1.0f};
    constexpr units::Mm WhiteBarWidth{CellWidth / 2.0f};

    constexpr units::Mm TotalHeight{
        CellHeight * NumberOfColumn + OpticalMarkHeight + BottomLineOffset
    };

    inline constexpr units::Point millimeter2Point(units::Mm millimeter){
        return millimeter * 2.83465f;
    }

    constexpr int EncodingBase{5};
    constexpr int EncodingBaseSquared{EncodingBase * EncodingBase};

    constexpr int SystemCommandStartIndex{12};
    constexpr int ChecksumIndex{15};

    constexpr int SetInstrumentOpcodeOffset{3};
    constexpr int SetVolumeOpcodeOffset{2};
    constexpr int SetArticulationOpcode{4};

    constexpr int TempoMultiplier{2};
    constexpr int TempoOffset{1};
    constexpr int VolumeMultiplier{10};
    constexpr int VolumeOffset{1};

    constexpr const char *PdfColorWhite{"1.0 1.0 1.0 rg"};
    constexpr const char *PdfColorRed{"1.0 0.0 0.0 rg"};
    constexpr const char *PdfColorGreen{"0.0 1.0 0.0 rg"};
    constexpr const char *PdfColorBlue{"0.0 0.0 1.0 rg"};
    constexpr const char *PdfColorBlack{"0.0 0.0 0.0 rg"};


} // namespace constants::enumerator