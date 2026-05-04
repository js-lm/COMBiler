#include "enumerator.hpp"

#include "pdf_canvas.hpp"
#include "pdf_document.hpp"

#include <external/tinyfiledialogs/tinyfiledialogs.h>

#include "debug_utilities.hpp"

void Enumerator::print(
    const program_states::ProjectData   &projectData, 
    const units::enumerator::Paper      paper,
    const units::Mm                     margin
){
    auto compiledPaperStrip{compileProjectData(projectData)};

    if(compiledPaperStrip.empty()) return;

    const std::string pattern{"*.pdf"};
    const char *filterPattern[1]{pattern.c_str()};

    const char *saveFilePath{tinyfd_saveFileDialog(
        "Export your paper strip",
        "paper_strip.pdf",
        1, filterPattern,
        "PDF document (*.pdf)"
    )};

    PdfDocument pdf{saveFilePath};

    PdfCanvas page{200};

    page.drawRectangle(100, 100, 100, 100, units::enumerator::Color::Red);
    page.drawRectangleLines(110, 110, 100, 100, units::enumerator::Color::Green);
    page.drawLines(25, 25, 25, 125, units::enumerator::Color::Blue);
    page.draw7SegmentDigit(9, 10, 10, 10, 20, 2, units::enumerator::Color::Black);

    DEBUG_PRINT("{}", page.stream());

    pdf.addPage(page, 200, 200);

    pdf.save();


    if(FileExists(saveFilePath)){
#if defined(__linux__)
        std::string command{"xdg-open \"" + std::string{saveFilePath} + "\""};
#elif defined(__APPLE__)
        std::string command{"open \"" + std::string{saveFilePath} + "\""};
#elif defined(_WIN32)
        std::string command{"start \"\" \"" + std::string{saveFilePath} + "\""};
#endif
        std::system(command.c_str());
    }

    return;
}