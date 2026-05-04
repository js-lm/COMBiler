#include "enumerator.hpp"

void Enumerator::drawStripSegment(
    PdfCanvas &canvas, 
    units::Mm startX, 
    units::Mm startY, 
    const std::vector<EncodedRow> &compiledPaperStrip, 
    size_t startIndex, 
    size_t count
){
    for(size_t currentIndex{0}; currentIndex < count; currentIndex++){
        size_t dataIndex{startIndex + currentIndex};
        if(dataIndex >= compiledPaperStrip.size()) break;

        units::Mm currentX{startX + (currentIndex * constants::enumerator::CellWidth)};

        drawOpticalMarkers(canvas, currentX, startY);
        drawCells(canvas, currentX, startY, compiledPaperStrip, dataIndex);
        drawBoundaryLine(canvas, currentX, startY);
    }
}

void Enumerator::drawOpticalMarkers(PdfCanvas &canvas, units::Mm x, units::Mm y){

    canvas.drawRectangle(
        x, y, 
        constants::enumerator::CellWidth, 
        constants::enumerator::OpticalMarkHeight, 
        units::enumerator::Color::Black
    );
    
    units::Mm whiteXOffset{(constants::enumerator::CellWidth - constants::enumerator::WhiteBarWidth) / 2.0f};
    canvas.drawRectangle(
        x + whiteXOffset, 
        y, 
        constants::enumerator::WhiteBarWidth, 
        constants::enumerator::OpticalMarkHeight, 
        units::enumerator::Color::White
    );
}

void Enumerator::drawBoundaryLine(PdfCanvas &canvas, units::Mm x, units::Mm y){

    units::Mm lineY{
        y + constants::enumerator::OpticalMarkHeight 
      + (constants::enumerator::NumberOfColumn * constants::enumerator::CellHeight) 
      + constants::enumerator::BottomLineOffset
    };
    
    canvas.drawLines(
        x, 
        lineY, 
        x + constants::enumerator::CellWidth, 
        lineY, units::enumerator::Color::Black
    );
}

void Enumerator::drawCells(
    PdfCanvas &canvas, 
    units::Mm x,
    units::Mm y,
    const std::vector<EncodedRow> &compiledPaperStrip,
    const size_t currentPaperStripRow
){
    if(currentPaperStripRow >= compiledPaperStrip.size()) return;

    const EncodedRow &rowData{compiledPaperStrip[currentPaperStripRow]};

    for(int columnIndex{0}; columnIndex < constants::enumerator::NumberOfColumn; columnIndex++){
        units::Mm cellY{
            y + constants::enumerator::OpticalMarkHeight 
          + (constants::enumerator::NumberOfColumn - 1 - columnIndex) * constants::enumerator::CellHeight
        };

        canvas.drawRectangle(
            x, 
            cellY, 
            constants::enumerator::CellWidth, 
            constants::enumerator::CellHeight, 
            rowData[columnIndex]
        );
        canvas.drawRectangleLines(
            x, 
            cellY, 
            constants::enumerator::CellWidth, 
            constants::enumerator::CellHeight, 
            units::enumerator::Color::Black
        );
    }
}