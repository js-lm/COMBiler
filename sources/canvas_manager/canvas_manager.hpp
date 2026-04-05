#pragma once

#include "program_states/context.hpp"

class CanvasManager{
private:
    // const MainWindow &mainWindow_;
    const program_states::ReadOnlyContext context_;

public:

    CanvasManager(program_states::ReadOnlyContext context)
        : context_{context}
    {}
    CanvasManager() = delete;
    ~CanvasManager() = default;

    void update();

private:
    void handleNoteTools();


private:
    void handlePageTools();

};