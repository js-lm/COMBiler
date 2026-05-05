#include "enumerator.hpp"

Enumerator::EncodedInstruction Enumerator::splitToBase5(int value, int opcodeOffset){
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
){
    units::Mm availableHeight{paperHeight - (paperMargin * 2.0f)};
    
    int stripPerSheet{static_cast<int>(availableHeight / constants::enumerator::TotalHeight)};

    // if(stripPerSheet_ < 1) stripPerSheet_ = 1;

    return stripPerSheet < 1 ? 1 : stripPerSheet;
    
}

int Enumerator::calculateRowsPerStripSegment(
    units::Mm paperWidth,
    units::Mm paperMargin
){
    units::Mm availableWidth{paperWidth - (paperMargin * 2.0f)};

    int rowsPerStripSegment{static_cast<int>(availableWidth / constants::enumerator::CellWidth)};

    // if(rowsPerStripSegment_ < 1) rowsPerStripSegment_ = 1;
 
    return rowsPerStripSegment < 1 ? 1 : rowsPerStripSegment;
}