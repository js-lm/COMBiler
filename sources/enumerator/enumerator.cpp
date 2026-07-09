#include "enumerator.hpp"

#ifdef PLATFORM_WEB
#include <emscripten.h>
#endif

#include "pdf_canvas.hpp"
#include "pdf_document.hpp"

#ifndef PLATFORM_WEB
#include <external/tinyfiledialogs/tinyfiledialogs.h>
#endif

#include "debug_utilities.hpp"

void Enumerator::print(
    const program_states::ProjectData   &projectData, 
    const units::enumerator::Paper      paper,
    const units::Mm                     margin
){
    const char *filterPattern[]{constants::enumerator::SaveFileExtensionPattern};

#ifdef PLATFORM_WEB
    const char *saveFilePath{constants::enumerator::DefaultFilename};
#else
    const char *saveFilePath{tinyfd_saveFileDialog(
        constants::enumerator::SaveDialogTitle,
        constants::enumerator::DefaultFilename,
        1, filterPattern,
        constants::enumerator::FilterDescription
    )};

    if(!saveFilePath) return;
#endif

    const auto compiledPaperStrip{compileProjectData(projectData)};
    if(compiledPaperStrip.empty()) return;

    PdfDocument pdf{saveFilePath};

    const auto &paperDimension{units::enumerator::getPaperDimension(paper)};

    generatePaperStrip(
        pdf,
        paperDimension.width,
        paperDimension.height,
        margin,
        compiledPaperStrip
    );

    pdf.save();

    if(FileExists(saveFilePath)){
#if defined(__EMSCRIPTEN__)
        EM_ASM_({
            var filePath = UTF8ToString($0);
            var fileName = UTF8ToString($1);
            try{
                var fileData = FS.readFile(filePath);
                var dataBlob = new Blob([fileData], {type: 'application/pdf'});
                var objectUrl = URL.createObjectURL(dataBlob);
                var anchorElement = document.createElement('a');
                anchorElement.href = objectUrl;
                anchorElement.download = fileName;
                document.body.appendChild(anchorElement);
                anchorElement.click();
                document.body.removeChild(anchorElement);
                URL.revokeObjectURL(objectUrl);
            }catch(error){
                // console.error('Failed to export PDF', error);
            }
        }, saveFilePath, constants::enumerator::DefaultFilename);

#elif defined(__linux__)
        std::string openCommand{"xdg-open \"" + std::string{saveFilePath} + "\""};
        std::system(openCommand.c_str());
#elif defined(__APPLE__)
        std::string openCommand{"open \"" + std::string{saveFilePath} + "\""};
        std::system(openCommand.c_str());
#elif defined(_WIN32)
        std::string openCommand{"start \"\" \"" + std::string{saveFilePath} + "\""};
        std::system(openCommand.c_str());
#endif
    }
}

void Enumerator::generatePaperStrip(
    PdfDocument &pdf, 
    const units::Mm width,
    const units::Mm height,
    const units::Mm margin,
    const std::vector<Enumerator::EncodedRow> &compiledPaperStrip
){
    int stripsPerSheet{calculateStripsPerSheet(height, margin)};
    int rowsPerStripSegment{calculateRowsPerStripSegment(width, margin)};
    int totalCellsPerSheet{stripsPerSheet * rowsPerStripSegment};

    units::Mm paperWidth{width};
    units::Mm paperHeight{height};

    /* try rotating the paper */ {

        int flippedStripsPerSheet{calculateStripsPerSheet(width, margin)};
        int flippedRowsPerStripSegment{calculateRowsPerStripSegment(height, margin)};
        int flippedTotalCellsPerSheet{flippedStripsPerSheet * flippedRowsPerStripSegment};

        if(flippedTotalCellsPerSheet > totalCellsPerSheet){
            stripsPerSheet = flippedStripsPerSheet;
            rowsPerStripSegment = flippedRowsPerStripSegment;
            paperWidth = height;
            paperHeight = width;
            totalCellsPerSheet = flippedTotalCellsPerSheet;

            DEBUG_PRINT("Rotated");
        }
    } /* try rotating the paper */

    DEBUG_PRINT("paperWidth:{}, paperHeight:{}", paperWidth, paperHeight);
    DEBUG_PRINT("stripsPerSheet:{}, rowsPerStripSegment:{}", stripsPerSheet, rowsPerStripSegment);

    if(totalCellsPerSheet < 1) return;

    // units::Mm availableHeight{paperHeight - (margin * 2.0f)};

    size_t totalRowCount{compiledPaperStrip.size()};
    size_t currentRowIndex{0};

    // size_t currentSegmentIndex{0};
    size_t totalSegmentCount{(totalRowCount + (stripsPerSheet * rowsPerStripSegment) - 1) / (stripsPerSheet * rowsPerStripSegment)};

    DEBUG_PRINT("totalRowCount:{}, totalSegmentCount:{}", totalRowCount, totalSegmentCount);

    int stripOrder{0};

    while(currentRowIndex < totalRowCount){
        PdfCanvas page{paperHeight};

        // center the segments
        size_t rowsRemainingOnPage{totalRowCount - currentRowIndex};
        int stripsOnCurrentPage{static_cast<int>(std::min<size_t>(
            static_cast<size_t>(stripsPerSheet), 
            (rowsRemainingOnPage + rowsPerStripSegment - 1) / rowsPerStripSegment
        ))};
        units::Mm totalContentHeight{
            (stripsOnCurrentPage * constants::enumerator::TotalHeight) + 
            ((stripsOnCurrentPage - 1) * constants::enumerator::GapBetweenSegments)
        };
        units::Mm startYOffset{(paperHeight - totalContentHeight) / 2.0f};

        // for(size_t currentSegmentIndex{0}; currentSegmentIndex < totalSegmentCount; currentSegmentIndex++){
        for(size_t currentStripIndex{0}; currentStripIndex < stripsPerSheet; currentStripIndex++){
            if(currentRowIndex >= totalRowCount) break;

            size_t itemsRemaining{totalRowCount - currentRowIndex};
            size_t itemsToDraw{std::min<size_t>(rowsPerStripSegment, itemsRemaining)};

            units::Mm startXPosition{(paperWidth - (itemsToDraw * constants::enumerator::CellWidth)) / 2.0f};
            units::Mm startYPosition{
                startYOffset 
              + (constants::enumerator::TotalHeight + constants::enumerator::GapBetweenSegments) * currentStripIndex
            };

            drawStripSegment(page, startXPosition, startYPosition, compiledPaperStrip, currentRowIndex, itemsToDraw);

            // digits
            units::Mm digitYPosition{
                startYPosition 
              + constants::enumerator::TotalHeight 
              + constants::enumerator::GapBetweenDigitAndStrip
            };
            
            // int numberToDraw{stripOrder};
            units::Mm currentDigitXPosition{startXPosition};
            
            if(stripOrder == 0){
                page.draw7SegmentDigit(
                    0, 
                    currentDigitXPosition, 
                    digitYPosition, 
                    constants::enumerator::DigitWidth, 
                    constants::enumerator::DigitHeight, 
                    constants::enumerator::DigitThickness, 
                    units::enumerator::Color::Black
                );
            }else{
                std::vector<int> digits{};
                int temporaryNumber{stripOrder};
                
                while(temporaryNumber > 0){
                    digits.push_back(temporaryNumber % 10);
                    temporaryNumber /= 10;
                }
                
                for(size_t currentDigitIndex{digits.size()}; currentDigitIndex --> 0;){
                    page.draw7SegmentDigit(
                        digits[currentDigitIndex], 
                        currentDigitXPosition, 
                        digitYPosition, 
                        constants::enumerator::DigitWidth, 
                        constants::enumerator::DigitHeight, 
                        constants::enumerator::DigitThickness, 
                        units::enumerator::Color::Black
                    );
                    currentDigitXPosition += constants::enumerator::DigitWidth + constants::enumerator::GapBetweenDigits;
                }
            }

            currentRowIndex += itemsToDraw;

            stripOrder++;
        }

        pdf.addPage(page, paperWidth, paperHeight);
    }
}