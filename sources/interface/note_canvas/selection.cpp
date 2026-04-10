#include "note_canvas.hpp"

#include <algorithm>

#include "constants.hpp"

using namespace interface;
using namespace constants::interface_layout::note_canvas;

void NoteCanvas::drawSelection(program_states::InterfaceContext &context){
    const auto &selectionArea{context.interface.clipboard.selectionArea};
    if(!selectionArea.has_value()) return;

    const auto &area{selectionArea.value()};
    if(area.widthInCells <= 0 || area.heightInCells <= 0) return;

    const float leftPositionX{
        context.interface.noteCanvas.gridArea.x
      + (static_cast<float>(area.topLeftColumnIndex) * context.interface.noteCanvas.columnWidth)
    };
    const float rightPositionX{
        context.interface.noteCanvas.gridArea.x
      + (static_cast<float>(area.topLeftColumnIndex + area.widthInCells) * context.interface.noteCanvas.columnWidth)
    };

    const float topPositionY{rowEdgeY(context, area.topLeftRowIndex)};
    const float bottomPositionY{rowEdgeY(context, area.topLeftRowIndex + area.heightInCells)};

    const auto drawDottedHorizontalLine{[&](float positionY){
        for(float currentPositionX{leftPositionX}; 
            currentPositionX < rightPositionX; 
            currentPositionX += selection::DashLengthInPixels + selection::GapLengthInPixels
        ){
            const float segmentEndPositionX{
                std::min(currentPositionX + selection::DashLengthInPixels, rightPositionX)
            };
            DrawLineEx(
                Vector2{currentPositionX, positionY},
                Vector2{segmentEndPositionX, positionY},
                selection::BorderThicknessInPixels,
                selection::BorderColor
            );
        }
    }};

    const auto drawDottedVerticalLine{[&](float positionX){
        for(float currentPositionY{topPositionY}; 
            currentPositionY < bottomPositionY; 
            currentPositionY += selection::DashLengthInPixels + selection::GapLengthInPixels
        ){
            const float segmentEndPositionY{
                std::min(currentPositionY + selection::DashLengthInPixels, bottomPositionY)
            };
            DrawLineEx(
                Vector2{positionX, currentPositionY},
                Vector2{positionX, segmentEndPositionY},
                selection::BorderThicknessInPixels,
                selection::BorderColor
            );
        }
    }};

    drawDottedHorizontalLine(topPositionY);
    drawDottedHorizontalLine(bottomPositionY);
    drawDottedVerticalLine(leftPositionX);
    drawDottedVerticalLine(rightPositionX);
}
