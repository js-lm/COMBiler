#include "main_window.hpp"

#include "constants.hpp"

#include <raylib.h>
#include <raygui.h>

#include "interface/interface.hpp"

#include "program_states/context.hpp"

#include <cstdlib>
#include <cstring>

#include "resources/styles/chromesthesia.h"
#include "resources/icons/iconset.h"

#include "debug_utilities.hpp"

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

    SetExitKey(KEY_NULL);

    // EnableEventWaiting(); // TODO: see how to add some delay to it

    SetWindowMinSize(
        constants::application_window::MinimalWidth, 
        constants::application_window::MinimalHeight
    );

    // SetTargetFPS(constants::application_window::DefaultFPS);

    std::memcpy(GuiGetIcons(), guiIcons, sizeof(guiIcons));
    GuiLoadStyleChromesthesia();
    
    initializeInterfaceRenderTexture();

    handleWindowSizeChangeEvent();

    actionCenter_ = std::make_unique<ActionCenter>();
    canvasManager_ = std::make_unique<CanvasManager>(getInterfaceContext());

    // stagedObserver_ = actionCenter_.getStagedObserver();
    systemState_.project.data = actionCenter_->getStagedObserver();

}

void MainWindow::update(){

    if(canvasManager_ && actionCenter_){
        canvasManager_->update(*actionCenter_);
    }

    handleEvents();
}

void MainWindow::draw(){
    auto context{getInterfaceContext()};

    BeginTextureMode(interfaceRenderTexture_); {
        // ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        ClearBackground(BLANK);

        const bool wasCommandWindowVisible{interfaceState_.prompts.isCommandWindowVisible};
        if(wasCommandWindowVisible) GuiLock();
        interface::Toolbar::draw(context);
        if(wasCommandWindowVisible) GuiUnlock();

        const bool shouldBlockUnderlyingUi{interfaceState_.prompts.isCommandWindowVisible};
        if(shouldBlockUnderlyingUi) GuiLock();
        interface::Sidebar::draw(context);
        interface::NavigationBar::draw(context);
        interface::NoteCanvas::draw(context);
        if(shouldBlockUnderlyingUi) GuiUnlock();

        // DEBUG_PRINT_IF_CHANGED(
        //     "isCommandWindowVisible: {}, wasCommandWindowVisible: {}, shouldBlockUnderlyingUi: {}",
        //     interfaceState_.prompts.isCommandWindowVisible,
        //     wasCommandWindowVisible, shouldBlockUnderlyingUi
        // );

        interface::Prompts::draw(context);
    } EndTextureMode();

    BeginDrawing(); {
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        const auto &window{systemState_.window};

        const Rectangle sourceRectangle{
            constants::application_window::Origin.x,
            constants::application_window::Origin.y,
            static_cast<float>(interfaceRenderTexture_.texture.width),
            -static_cast<float>(interfaceRenderTexture_.texture.height)
        };

        const Rectangle destinationRectangle{
            window.isIntegerScaling
                ? (static_cast<float>(GetScreenWidth()) - (static_cast<float>(window.interfaceRenderTextureWidth) * window.scaleFactor)) / 2.0f
                : constants::application_window::Origin.x,
            window.isIntegerScaling
                ? (static_cast<float>(GetScreenHeight()) - (static_cast<float>(window.interfaceRenderTextureHeight) * window.scaleFactor)) / 2.0f
                : constants::application_window::Origin.y,
            window.isIntegerScaling
                ? static_cast<float>(window.interfaceRenderTextureWidth) * window.scaleFactor
                : static_cast<float>(GetScreenWidth()),
            window.isIntegerScaling
                ? static_cast<float>(window.interfaceRenderTextureHeight) * window.scaleFactor
                : static_cast<float>(GetScreenHeight())
        };

        DrawTexturePro(
            interfaceRenderTexture_.texture,
            sourceRectangle,
            destinationRectangle,
            constants::application_window::Origin,
            .0f,
            WHITE
        );
#ifdef DEBUG
        DrawFPS(0, 0);
#endif

    } EndDrawing();
}

void MainWindow::initializeInterfaceRenderTexture(){
    systemState_.window.interfaceRenderTextureWidth = GetScreenWidth();
    systemState_.window.interfaceRenderTextureHeight = GetScreenHeight();
    interfaceRenderTexture_ = LoadRenderTexture(
        systemState_.window.interfaceRenderTextureWidth, 
        systemState_.window.interfaceRenderTextureHeight
    );

    applyTextureFilter();
}

void MainWindow::unloadInterfaceRenderTexture(){
    if(interfaceRenderTexture_.id == 0) return;

    UnloadRenderTexture(interfaceRenderTexture_);
    interfaceRenderTexture_ = RenderTexture2D{};
}