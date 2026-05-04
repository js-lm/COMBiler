#include "pdf_document.hpp"

#include <fmt/core.h>

PdfDocument::PdfDocument(const std::string &filepath)
    : file_{filepath, std::ios::binary}
    , objectCount_{constants::enumerator::pdf::InitialObjectCount}
{
    file_ << "%PDF-1.4\n";

    xrefOffsets_.resize(objectCount_, 0);
}

int PdfDocument::writeObjectStart(){

    if(objectCount_ >= xrefOffsets_.size()){
        xrefOffsets_.resize(objectCount_ + 1);
    }

    xrefOffsets_[objectCount_] = static_cast<size_t>(file_.tellp());
    file_ << fmt::format("{} 0 obj\n", objectCount_);

    int currentId{objectCount_};
    objectCount_++;

    return currentId;
}

void PdfDocument::writeObjectEnd(){
    file_ << "endobj\n";
}

void PdfDocument::addPage(const PdfCanvas &canvas, units::Mm width, units::Mm height){
    units::Point pointWidth{constants::enumerator::millimeter2Point(width)};
    units::Point pointHeight{constants::enumerator::millimeter2Point(height)};

    std::string stream{canvas.stream()};

    int lengthObjectId{writeObjectStart()};
    file_ << fmt::format("{}\n", stream.size());
    writeObjectEnd();

    int contentObjectId{writeObjectStart()};
    file_ << fmt::format("<< /Length {} 0 R >>\n", lengthObjectId);
    file_ << "stream\n";
    file_ << stream;

    if(stream.empty() || stream.back() != '\n'){
        file_ << "\n";
    }

    file_ << "endstream\n";
    writeObjectEnd();

    int pageObjectId{writeObjectStart()};

    file_ << "<< /Type /Page\n";
    file_ << fmt::format("/Parent {} 0 R\n", constants::enumerator::pdf::PagesObjectId);
    file_ << fmt::format("/MediaBox [0 0 {:.2f} {:.2f}]\n", pointWidth, pointHeight);
    file_ << fmt::format("/Contents {} 0 R\n", contentObjectId);
    file_ << ">>\n";

    writeObjectEnd();

    pageObjectIds_.push_back(pageObjectId);
}

void PdfDocument::save(){

    xrefOffsets_[constants::enumerator::pdf::PagesObjectId] = static_cast<size_t>(file_.tellp());
    file_ << fmt::format("{} 0 obj\n", constants::enumerator::pdf::PagesObjectId);
    file_ << "<< /Type /Pages\n";
    file_ << "/Kids [";

    for(int identifier : pageObjectIds_){
        file_ << fmt::format("{} 0 R ", identifier);
    }
    file_ << "]\n";
    file_ << fmt::format("/Count {}\n", pageObjectIds_.size());
    file_ << ">>\n";
    writeObjectEnd();

    int catalogObjectId{writeObjectStart()};
    file_ << fmt::format("<< /Type /Catalog /Pages {} 0 R >>\n", constants::enumerator::pdf::PagesObjectId);
    writeObjectEnd();

    size_t xrefStart{static_cast<size_t>(file_.tellp())};
    file_ << "xref\n";
    file_ << fmt::format("0 {}\n", objectCount_);
    file_ << "0000000000 65535 f \n";

    for(int currentPage{1}; currentPage < objectCount_; currentPage++){
        file_ << fmt::format("{:010} 00000 n \n", xrefOffsets_[currentPage]);
    }

    file_ << "trailer\n";
    file_ << fmt::format("<< /Size {}\n", objectCount_);
    file_ << fmt::format("/Root {} 0 R >>\n", catalogObjectId);
    file_ << "startxref\n";
    file_ << fmt::format("{}\n", xrefStart);
    file_ << "%%EOF\n";

    file_.close();
}