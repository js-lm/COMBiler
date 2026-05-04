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