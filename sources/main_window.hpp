#pragma once

#include "program_states/interface.hpp"
#include "program_states/system.hpp"
#include "program_states/layout.hpp"

#include <raylib.h>

class MainWindow{
private:
    program_states::System      systemState_{};
    program_states::Interface   interfaceState_{};
    program_states::Layout      layoutState_{};

private:
    RenderTexture2D interfaceRenderTexture_{};

public:
    MainWindow() = default;
    ~MainWindow() = default;

    int run();

private:

    void initialize();

    void update();
    void draw();

private:
    void initializeInterfaceRenderTexture();
    void unloadInterfaceRenderTexture();

    void handleEvents();

    void handleWindowSizeChangeEvent();
    

};