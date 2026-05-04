#pragma once

#include <fstream>
#include <vector>

#include "pdf_canvas.hpp"

#include "aliases.hpp"

class PdfDocument{
private:
    std::ofstream file_;

    std::vector<size_t> xrefOffsets_;

    int objectCount_;

    std::vector<int> pageObjectIds_;

public:
    PdfDocument(const std::string &filepath);
    PdfDocument() = delete;
    ~PdfDocument() = default;

    void addPage(const PdfCanvas &canvas, units::Mm width, units::Mm height);

    void save();

private:
    int writeObjectStart();
    void writeObjectEnd();
};