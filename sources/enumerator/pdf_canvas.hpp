#pragma once

#include <string>

#include "constants.hpp"
#include "aliases.hpp"

class PdfCanvas{
private:
    std::string stream_;

    units::Mm pageHeight_;

public:
    void drawRectangle(units::Mm x, units::Mm y, units::Mm width, units::Mm height, units::enumerator::Color color);
    void drawRectangleLines(units::Mm x, units::Mm y, units::Mm width, units::Mm height, units::enumerator::Color color);
    void drawLines(units::Mm startX, units::Mm startY, units::Mm endX, units::Mm endY, units::enumerator::Color color);
    void draw7SegmentDigit(int digit, units::Mm x, units::Mm y);

private:
    std::string toPdfColor(const units::enumerator::Color color) const;
};