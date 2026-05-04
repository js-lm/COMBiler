#pragma once

#include <string>

#include "constants.hpp"
#include "aliases.hpp"

class PdfCanvas{
private:
    std::string stream_;

    units::Mm pageHeight_;

public:
    PdfCanvas(units::Mm pageHeight) 
        : pageHeight_{pageHeight}
    {}
    PdfCanvas() = delete;
    ~PdfCanvas() = default;

    const std::string &stream() const{ return stream_;}

public:

    void drawRectangle(units::Mm x, units::Mm y, units::Mm width, units::Mm height, units::enumerator::Color color);
    void drawRectangleLines(units::Mm x, units::Mm y, units::Mm width, units::Mm height, units::enumerator::Color color);
    void drawLines(units::Mm startX, units::Mm startY, units::Mm endX, units::Mm endY, units::enumerator::Color color);
    void draw7SegmentDigit(int digit, units::Mm x, units::Mm y, units::Mm width, units::Mm height, units::Mm thickness, units::enumerator::Color color);

private:
    std::string toPdfFillColor(const units::enumerator::Color color) const;
    std::string toPdfStrokeColor(const units::enumerator::Color color) const;

};