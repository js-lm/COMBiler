#include "note_canvas.hpp"

#include <algorithm>

#include "constants.hpp"

using namespace interface;
using namespace constants::interface_layout::note_canvas;

void NoteCanvas::drawSelection(program_states::InterfaceContext &context){
    program_states::Interface::Clipboard::SelectionArea area{};

    if(context.interface.clipboard.isPasteModeEnabled
    && context.interface.clipboard.hasCopiedSelection
    && context.interface.noteCanvas.cursorPosition.has_value()
    ){
        const auto &cursor{context.interface.noteCanvas.cursorPosition.value()};

        area.widthInCells = context.interface.clipboard.copiedWidthInCells;
        area.heightInCells = context.interface.clipboard.copiedHeightInCells;

        area.topLeftColumnIndex = cursor.noteIndex - (area.widthInCells / 2);

        const int centerRowIndex{
            FirstNoteOffsetFromC0 + (NumberOfRow - 1) - static_cast<int>(cursor.note)
        };
        area.topLeftRowIndex = centerRowIndex - (area.heightInCells / 2);
    }else{
        const auto &selectionArea{context.interface.clipboard.selectionArea};
        if(!selectionArea.has_value()) return;
        area = selectionArea.value();
    }

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
