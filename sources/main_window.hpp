#pragma once

#include "system_states.hpp"

#include <raylib.h>

class MainWindow{
private:
    SystemState systemState_;

private:
    RenderTexture2D interfaceRenderTexture_{};
    int interfaceRenderTextureWidth_{0};
    int interfaceRenderTextureHeight_{0};

    


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