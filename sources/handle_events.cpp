#include "main_window.hpp"

void MainWindow::handleEvents(){
    handleWindowSizeChangeEvent();



}

void MainWindow::handleWindowSizeChangeEvent(){
    const int currentWindowWidth{GetScreenWidth()};
    const int currentWindowHeight{GetScreenHeight()};

    if(currentWindowWidth == interfaceRenderTextureWidth_
    && currentWindowHeight == interfaceRenderTextureHeight_
    ){
        return;
    }

    unloadInterfaceRenderTexture();

    interfaceRenderTextureWidth_ = currentWindowWidth;
    interfaceRenderTextureHeight_ = currentWindowHeight;
    interfaceRenderTexture_ = LoadRenderTexture(interfaceRenderTextureWidth_, interfaceRenderTextureHeight_);

}