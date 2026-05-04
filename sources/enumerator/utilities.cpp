#include "enumerator.hpp"

Enumerator::EncodedInstruction Enumerator::splitToBase5(int value, int opcodeOffset) const{
    int operationCode{(value / constants::enumerator::EncodingBaseSquared) + opcodeOffset};
    int remainder{value % constants::enumerator::EncodingBaseSquared};
    int immediateDigit1{remainder / constants::enumerator::EncodingBase};
    int immediateDigit2{remainder % constants::enumerator::EncodingBase};

    return {
        static_cast<units::enumerator::Color>(operationCode),
        static_cast<units::enumerator::Color>(immediateDigit1),
        static_cast<units::enumerator::Color>(immediateDigit2)
    };
}

int Enumerator::calculateStripsPerSheet(
    units::Mm paperHeight,
    units::Mm paperMargin
) const{
    units::Mm availableHeight{paperHeight - (paperMargin * 2.0f)};
    
    int stripPerSheet{static_cast<int>(availableHeight / constants::enumerator::TotalHeight)};

    // if(stripPerSheet_ < 1) stripPerSheet_ = 1;

    return stripPerSheet < 1 ? 1 : stripPerSheet;
    
}

int Enumerator::calculatePerStripSegment(
    units::Mm paperWidth,
    units::Mm paperMargin
) const{
    units::Mm availableWidth{paperWidth - (paperMargin * 2.0f)};

    int rowsPerStripSegment{static_cast<int>(availableWidth / constants::enumerator::CellWidth)};

    // if(rowsPerStripSegment_ < 1) rowsPerStripSegment_ = 1;
 
    return rowsPerStripSegment < 1 ? 1 : rowsPerStripSegment;
}

std::string Enumerator::toPdfColor(const units::enumerator::Color color) const{
    switch(color){
    case units::enumerator::Color::White:   return constants::enumerator::PdfColorWhite;
    case units::enumerator::Color::Red:     return constants::enumerator::PdfColorRed;
    case units::enumerator::Color::Green:   return constants::enumerator::PdfColorGreen;
    case units::enumerator::Color::Blue:    return constants::enumerator::PdfColorBlue;
    case units::enumerator::Color::Black:   return constants::enumerator::PdfColorBlack;
    }
}