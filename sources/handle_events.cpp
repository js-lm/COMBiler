#include "main_window.hpp"

void MainWindow::handleEvents(){
    handleWindowSizeChangeEvent();



}

void MainWindow::handleWindowSizeChangeEvent(){
    const int currentWindowWidth{GetScreenWidth()};
    const int currentWindowHeight{GetScreenHeight()};

    if(currentWindowWidth == systemState_.window.interfaceRenderTextureWidth
    && currentWindowHeight == systemState_.window.interfaceRenderTextureHeight
    ){
        return;
    }

    unloadInterfaceRenderTexture();

    systemState_.window.interfaceRenderTextureWidth  = currentWindowWidth;
    systemState_.window.interfaceRenderTextureHeight = currentWindowHeight;


    interfaceRenderTexture_ = LoadRenderTexture(
        systemState_.window.interfaceRenderTextureWidth, 
        systemState_.window.interfaceRenderTextureHeight
    );

}