#include "pdf_canvas.hpp"

#include <fmt/core.h>

std::string PdfCanvas::toPdfFillColor(const units::enumerator::Color color) const{
    switch(color){
    case units::enumerator::Color::White:   return constants::enumerator::pdf::FillColorWhite;
    case units::enumerator::Color::Red:     return constants::enumerator::pdf::FillColorRed;
    case units::enumerator::Color::Green:   return constants::enumerator::pdf::FillColorGreen;
    case units::enumerator::Color::Blue:    return constants::enumerator::pdf::FillColorBlue;
    case units::enumerator::Color::Black:   return constants::enumerator::pdf::FillColorBlack;
    }
    return "";
}

std::string PdfCanvas::toPdfStrokeColor(const units::enumerator::Color color) const{
    switch(color){
    case units::enumerator::Color::White:   return constants::enumerator::pdf::StrokeColorWhite;
    case units::enumerator::Color::Red:     return constants::enumerator::pdf::StrokeColorRed;
    case units::enumerator::Color::Green:   return constants::enumerator::pdf::StrokeColorGreen;
    case units::enumerator::Color::Blue:    return constants::enumerator::pdf::StrokeColorBlue;
    case units::enumerator::Color::Black:   return constants::enumerator::pdf::StrokeColorBlack;
    }
    return "";
}

void PdfCanvas::drawRectangle(units::Mm x, units::Mm y, units::Mm width, units::Mm height, units::enumerator::Color color){
    stream_ += fmt::format(
        "{} {:.2f} {:.2f} {:.2f} {:.2f} re f\n", 
        toPdfFillColor(color), 
        constants::enumerator::millimeter2Point(x), 
        constants::enumerator::millimeter2Point(pageHeight_ - y - height), 
        constants::enumerator::millimeter2Point(width), 
        constants::enumerator::millimeter2Point(height)
    );
}

void PdfCanvas::drawRectangleLines(units::Mm x, units::Mm y, units::Mm width, units::Mm height, units::Mm thickness, units::enumerator::Color color){
    stream_ += fmt::format(
        "{:.2f} w {} {:.2f} {:.2f} {:.2f} {:.2f} re S\n", 
        constants::enumerator::millimeter2Point(thickness),
        toPdfStrokeColor(color), 
        constants::enumerator::millimeter2Point(x), 
        constants::enumerator::millimeter2Point(pageHeight_ - y - height), 
        constants::enumerator::millimeter2Point(width), 
        constants::enumerator::millimeter2Point(height)
    );
}

void PdfCanvas::drawLines(units::Mm startX, units::Mm startY, units::Mm endX, units::Mm endY, units::Mm thickness, units::enumerator::Color color){
    stream_ += fmt::format(
        "{:.2f} w {} {:.2f} {:.2f} m {:.2f} {:.2f} l S\n", 
        constants::enumerator::millimeter2Point(thickness),
        toPdfStrokeColor(color), 
        constants::enumerator::millimeter2Point(startX), 
        constants::enumerator::millimeter2Point(pageHeight_ - startY), 
        constants::enumerator::millimeter2Point(endX), 
        constants::enumerator::millimeter2Point(pageHeight_ - endY)
    );
}

void PdfCanvas::draw7SegmentDigit(int digit, units::Mm x, units::Mm y, units::Mm width, units::Mm height, units::Mm thickness, units::enumerator::Color color){
    if(digit < 0 || digit > 9) return;
    const bool *segments{constants::enumerator::sevenSegment[digit]};

    if(segments[0]) drawRectangle(x, y, width, thickness, color);
    if(segments[1]) drawRectangle(x + width - thickness, y, thickness, height * .5f, color);
    if(segments[2]) drawRectangle(x + width - thickness, y + height * .5f, thickness, height * .5f, color);
    if(segments[3]) drawRectangle(x, y + height - thickness, width, thickness, color);
    if(segments[4]) drawRectangle(x, y + height * .5f, thickness, height * .5f, color);
    if(segments[5]) drawRectangle(x, y, thickness, height * .5f, color);
    if(segments[6]) drawRectangle(x, y + height * .5f - thickness * .5f, width, thickness, color);
}
