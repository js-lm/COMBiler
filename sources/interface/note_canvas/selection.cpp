#include "note_canvas.hpp"

#include <algorithm>

#include "constants.hpp"

using namespace interface;
namespace canvas_constants = constants::interface_layout::note_canvas;

void NoteCanvas::drawSelection(program_states::InterfaceContext &context){
    program_states::Interface::Clipboard::SelectionArea area{};

    if(context.interface.clipboard.isPasteModeEnabled
    && context.interface.clipboard.hasCopiedSelection
    && context.interface.clipboard.hasPasteAnchor
    ){
        const int anchorNoteIndex{context.interface.clipboard.pasteAnchorNoteIndex};
        const int centerRowIndex{context.interface.clipboard.pasteAnchorCenterRowIndex};

        area.widthInCells = context.interface.clipboard.copiedWidthInCells;
        area.heightInCells = context.interface.clipboard.copiedHeightInCells;

        area.topLeftColumnIndex = anchorNoteIndex - (area.widthInCells / 2);
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
            currentPositionX += canvas_constants::selection::DashLengthInPixels + canvas_constants::selection::GapLengthInPixels
        ){
            const float segmentEndPositionX{
                std::min(currentPositionX + canvas_constants::selection::DashLengthInPixels, rightPositionX)
            };
            DrawLineEx(
                Vector2{currentPositionX, positionY},
                Vector2{segmentEndPositionX, positionY},
                canvas_constants::selection::BorderThicknessInPixels,
                canvas_constants::selection::BorderColor
            );
        }
    }};

    const auto drawDottedVerticalLine{[&](float positionX){
        for(float currentPositionY{topPositionY}; 
            currentPositionY < bottomPositionY; 
            currentPositionY += canvas_constants::selection::DashLengthInPixels + canvas_constants::selection::GapLengthInPixels
        ){
            const float segmentEndPositionY{
                std::min(currentPositionY + canvas_constants::selection::DashLengthInPixels, bottomPositionY)
            };
            DrawLineEx(
                Vector2{positionX, currentPositionY},
                Vector2{positionX, segmentEndPositionY},
                canvas_constants::selection::BorderThicknessInPixels,
                canvas_constants::selection::BorderColor
            );
        }
    }};

    drawDottedHorizontalLine(topPositionY);
    drawDottedHorizontalLine(bottomPositionY);
    drawDottedVerticalLine(leftPositionX);
    drawDottedVerticalLine(rightPositionX);
}
