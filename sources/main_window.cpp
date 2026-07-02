#include "main_window.hpp"

#include "constants.hpp"

#include <raylib.h>
#include <raygui.h>

#include "interface/interface.hpp"

#include "program_states/context.hpp"
#include "program_states/project_data.hpp"

#include <cstdlib>
#include <cstring>
#include <chrono>
#include <thread>
#include <vector>
#include <string>

#include "resources/styles/chromesthesia.h"
#include "resources/icons/iconset.h"

#include "debug_utilities.hpp"

#include "utilities/project_utilities.hpp"

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

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);

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

    SetTargetFPS(constants::application_window::DefaultFPS);

    std::memcpy(GuiGetIcons(), guiIcons, sizeof(guiIcons));
    GuiLoadStyleChromesthesia();
    
    initializeInterfaceRenderTexture();

    handleWindowSizeChangeEvent();

    actionCenter_ = std::make_unique<ActionCenter>();
    canvasManager_ = std::make_unique<CanvasManager>(getInterfaceContext());
    midiManager_ = std::make_unique<MidiManager>(getMidiContext());
    midiManager_->initialization();
    playbackManager_ = std::make_unique<PlaybackManager>(getMidiContext());
    serializer_ = std::make_unique<Serializer>();
    // enumerator_ = std::make_unique<Enumerator>();

    // stagedObserver_ = actionCenter_.getStagedObserver();
    systemState_.project.data = actionCenter_->getStagedObserver();

}

void MainWindow::update(){

    handleEvents();

    // midiManager_->update();
    canvasManager_->update(*actionCenter_, *midiManager_);
    playbackManager_->update(*midiManager_);

    if(auto projectData{utilities::projectDataWithPagesFrom(systemState_)}){
        projectData->transient.currentPageNumber = systemState_.project.currentPage;
        projectData->transient.selectedChannelListViewIndex = interfaceState_.sidebar.selectedChannelListViewIndex;
    }
}

void MainWindow::draw(){
    auto context{getInterfaceContext()};

    BeginTextureMode(interfaceRenderTexture_); {
        // ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        ClearBackground(BLANK);

        const bool shouldDisableToolbar(machineState_.isPlaying);
        if(shouldDisableToolbar) GuiDisable();

        const bool wasPromptWindowVisible{
            interfaceState_.prompts.isCommandWindowVisible
         || interfaceState_.prompts.isConstantsManagerWindowVisible
         || interfaceState_.prompts.isConstantsManagerWarningWindowVisible
         || interfaceState_.prompts.isConstantsManagerInfoWindowVisible
         || interfaceState_.prompts.isInfoWindowVisible
         || interfaceState_.prompts.isOverwritePromptVisible
        };
        if(wasPromptWindowVisible) GuiLock();
        interface::Toolbar::draw(context);
        if(wasPromptWindowVisible) GuiUnlock();
        if(shouldDisableToolbar) GuiEnable();

        const bool shouldBlockUnderlyingUi{
            wasPromptWindowVisible 
        //  || machineState_.isPlaying
        };
        if(shouldBlockUnderlyingUi) GuiLock();
        interface::Sidebar::draw(context);
        interface::NavigationBar::draw(context);
        interface::NoteCanvas::draw(context);
        if(shouldBlockUnderlyingUi) GuiUnlock();

        // DEBUG_PRINT_IF_CHANGED(
        //     " shouldBlockUnderlyingUi: {}",
        //     shouldBlockUnderlyingUi
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
        // DrawFPS(0, 0);
        DrawText("DEBUG MODE", 0, 0, 20, GREEN);
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
