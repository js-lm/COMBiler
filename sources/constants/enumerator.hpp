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
    constexpr units::Mm CellLineWidth{.1f};
    constexpr units::Mm LineWidth{.25f};
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
    // constexpr int VolumeMultiplier{10};
    constexpr int VolumeOffset{1};

    constexpr const bool sevenSegment[][7]{
        {1,1,1,1,1,1,0}, // 0
        {0,1,1,0,0,0,0}, // 1
        {1,1,0,1,1,0,1}, // 2
        {1,1,1,1,0,0,1}, // 3
        {0,1,1,0,0,1,1}, // 4
        {1,0,1,1,0,1,1}, // 5
        {1,0,1,1,1,1,1}, // 6
        {1,1,1,0,0,0,0}, // 7
        {1,1,1,1,1,1,1}, // 8
        {1,1,1,1,0,1,1}  // 9
    };

    namespace pdf{

        constexpr const char *FillColorWhite{"1.0 1.0 1.0 rg"};
        constexpr const char *StrokeColorWhite{"1.0 1.0 1.0 RG"};
        constexpr const char *FillColorRed{"1.0 0.0 0.0 rg"};
        constexpr const char *StrokeColorRed{"1.0 0.0 0.0 RG"};
        constexpr const char *FillColorGreen{"0.0 1.0 0.0 rg"};
        constexpr const char *StrokeColorGreen{"0.0 1.0 0.0 RG"};
        constexpr const char *FillColorBlue{"0.0 0.0 1.0 rg"};
        constexpr const char *StrokeColorBlue{"0.0 0.0 1.0 RG"};
        constexpr const char *FillColorBlack{"0.0 0.0 0.0 rg"};
        constexpr const char *StrokeColorBlack{"0.0 0.0 0.0 RG"};

        // constexpr const char *FormatRectangle{"{} {:.2f} {:.2f} {:.2f} {:.2f} re f\n"};
        // constexpr const char *FormatRectangleLines{"{} {:.2f} {:.2f} {:.2f} {:.2f} re S\n"};
        // constexpr const char *FormatLine{"{} {:.2f} {:.2f} m {:.2f} {:.2f} l S\n"};

        // constexpr const char *Version{"%PDF-1.4\n"};
        // constexpr const char *ObjectStart{"{} 0 obj\n"};
        // constexpr const char *ObjectEnd{"endobj\n"};
        // constexpr const char *StreamSize{"{}\n"};
        // constexpr const char *Length{"<< /Length {} 0 R >>\n"};
        // constexpr const char *StreamStart{"stream\n"};
        // constexpr const char *StreamEnd{"endstream\n"};
        // constexpr const char *TypePage{"<< /Type /Page\n"};
        // constexpr const char *PageParent{"/Parent {} 0 R\n"};
        // constexpr const char *MediaBox{"/MediaBox [0 0 {:.2f} {:.2f}]\n"};
        // constexpr const char *Contents{"/Contents {} 0 R\n"};
        // constexpr const char *DictionaryEnd{">>\n"};
        // constexpr const char *ObjectReference{"{} 0 R "};
        // constexpr const char *Newline{"\n"};

        constexpr int PagesObjectId{1};
        constexpr int InitialObjectCount{2};

    } // namespace pdf

    constexpr const char *SaveDialogTitle{"Export Your Project"};

    constexpr const char *SaveFileExtensionPattern{"*.pdf"};
    constexpr const char *FilterDescription{"PDF document (*.pdf)"};

    constexpr const char *DefaultFilename{"COMBosition.pdf"};

    constexpr units::Mm GapBetweenSegments{10.0f};
    constexpr units::Mm GapBetweenDigitAndStrip{1.0f};
    constexpr units::Mm GapBetweenDigits{1.0f};

    constexpr units::Mm DigitWidth{4.0f};
    constexpr units::Mm DigitHeight{4.0f};
    constexpr units::Mm DigitThickness{.5f};




} // namespace constants::enumerator