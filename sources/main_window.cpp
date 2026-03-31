#include "main_window.hpp"

#include "constants.hpp"

#include <raylib.h>
#include <raygui.h>

#include "interface/interface.hpp"

#include "program_states/context.hpp"

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
    program_states::Context context{
        .system     {systemState_},
        .layout     {layoutState_},
        .interface  {interfaceState_}
    };

    BeginTextureMode(interfaceRenderTexture_); {
        // ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        ClearBackground(BLANK);

        interface::Toolbar::draw(context);
        interface::Sidebar::draw(context);
        interface::NavigationBar::draw(context);
        interface::NoteCanvas::draw(context);
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
    systemState_.window.interfaceRenderTextureWidth = GetScreenWidth();
    systemState_.window.interfaceRenderTextureHeight = GetScreenHeight();
    interfaceRenderTexture_ = LoadRenderTexture(
        systemState_.window.interfaceRenderTextureWidth, 
        systemState_.window.interfaceRenderTextureHeight
    );
}

void MainWindow::unloadInterfaceRenderTexture(){
    if(interfaceRenderTexture_.id == 0) return;

    UnloadRenderTexture(interfaceRenderTexture_);
    interfaceRenderTexture_ = RenderTexture2D{};
}