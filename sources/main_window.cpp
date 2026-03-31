#include "main_window.hpp"

#include "constants.hpp"

#include <raylib.h>
#include <raygui.h>

#include "interface/interface.hpp"

int MainWindow::run(){

    initialize();

    while(!WindowShouldClose()){
        draw();
        update();
    }

    unloadInterfaceRenderTexture();
    CloseWindow();

    return 0;
}

void MainWindow::initialize(){

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(
        constants::application_window::DefaultWidth, 
        constants::application_window::DefaultHeight, 
        constants::application_window::Title
    );

    SetWindowMinSize(
        constants::application_window::MinimalWidth, 
        constants::application_window::MinimalHeight
    );

    SetTargetFPS(constants::application_window::DefaultFPS);

    initializeInterfaceRenderTexture();

}

void MainWindow::update(){

    handleEvents();


}

void MainWindow::draw(){

    BeginTextureMode(interfaceRenderTexture_); {
        // ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        ClearBackground(BLANK);

        interface::Toolbar::draw(systemState_);
        interface::Sidebar::draw(systemState_);
        interface::NavigationBar::draw(systemState_);
        interface::NoteCanvas::draw(systemState_);
    } EndTextureMode();

    BeginDrawing(); {
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        const Rectangle sourceRectangle{
            constants::application_window::Origin.x,
            constants::application_window::Origin.y,
            static_cast<float>(interfaceRenderTexture_.texture.width),
            -static_cast<float>(interfaceRenderTexture_.texture.height)
        };
        
        DrawTextureRec(interfaceRenderTexture_.texture, sourceRectangle, constants::application_window::Origin, WHITE);
    } EndDrawing();
}

void MainWindow::initializeInterfaceRenderTexture(){
    interfaceRenderTextureWidth_ = GetScreenWidth();
    interfaceRenderTextureHeight_ = GetScreenHeight();
    interfaceRenderTexture_ = LoadRenderTexture(interfaceRenderTextureWidth_, interfaceRenderTextureHeight_);
}

void MainWindow::unloadInterfaceRenderTexture(){
    if(interfaceRenderTexture_.id == 0) return;

    UnloadRenderTexture(interfaceRenderTexture_);
    interfaceRenderTexture_ = RenderTexture2D{};
}