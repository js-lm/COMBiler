#include "main_window.hpp"

#include "constants.hpp"

#include <raylib.h>
#include <raygui.h>

#include "interface/interface.hpp"

#include "program_states/context.hpp"
#include "program_states/project_data.hpp"

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <GLFW/glfw3.h>
#endif

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

#ifdef PLATFORM_WEB
    emscripten_set_main_loop_arg(MainWindow::webMainLoopCallback, this, 0, 1);
#else
    while(!WindowShouldClose()){
        draw();
        update();
    }
#endif

    unloadInterfaceRenderTexture();
    CloseWindow();

    return 0;
}

#ifdef PLATFORM_WEB
void MainWindow::webMainLoopCallback(void *argument){
    MainWindow *self{static_cast<MainWindow*>(argument)};
    self->webLoopBody();
}

void MainWindow::webLoopBody(){
    EM_ASM({
        var canvasId = UTF8ToString($0);
        var canvas = document.getElementById(canvasId);
        if(document.activeElement !== canvas) canvas.focus();
    }, constants::application_window::CanvasId);
    static int previousWidth{0};
    static int previousHeight{0};
    int browserWidth{EM_ASM_INT({ return window.innerWidth;})};
    int browserHeight{EM_ASM_INT({ return window.innerHeight;})};
    
    if(browserWidth != previousWidth || browserHeight != previousHeight){
        previousWidth = browserWidth;
        previousHeight = browserHeight;
        emscripten_set_canvas_element_size(constants::application_window::CanvasSelector, browserWidth, browserHeight);
        SetWindowSize(browserWidth, browserHeight);
        handleWindowSizeChangeEvent();
    }

    update();
    draw();
}
#endif

void MainWindow::initialize(){

#ifdef PLATFORM_WEB
    EM_ASM_({
        console.log("Mounting IDBFS...");

        var mountPoint = UTF8ToString($0);
        FS.mkdir(mountPoint);
        FS.mount(IDBFS, {}, mountPoint);
        FS.syncfs(true, function(error){
            if(error) console.error("IDBFS initial sync error:", error);
            else console.log("IDBFS initial sync complete. Calling onIdbfsReady...");

            Module.ccall('onIdbfsReady', 'void', [], []);
        });
    }, constants::serializer::IdbfsMountPoint);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
#else
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);
#endif

#ifdef PLATFORM_WEB
    int canvasWidth{0};
    int canvasHeight{0};
    emscripten_get_canvas_element_size(constants::application_window::CanvasSelector, &canvasWidth, &canvasHeight);
    
    if(canvasWidth <= 0 || canvasHeight <= 0){
        canvasWidth = constants::application_window::DefaultWidth;
        canvasHeight = constants::application_window::DefaultHeight;
    }
    InitWindow(canvasWidth, canvasHeight, constants::application_window::Title);
#else
    InitWindow(
        constants::application_window::DefaultWidth, 
        constants::application_window::DefaultHeight, 
        constants::application_window::Title
    );
#endif

#ifndef PLATFORM_WEB
    SetExitKey(KEY_NULL);
#endif

    // EnableEventWaiting(); // TODO: see how to add some delay to it

#ifndef PLATFORM_WEB
    SetWindowMinSize(
        constants::application_window::MinimalWidth, 
        constants::application_window::MinimalHeight
    );

    SetTargetFPS(constants::application_window::DefaultFPS);
#endif

    std::memcpy(GuiGetIcons(), guiIcons, sizeof(guiIcons));
    GuiLoadStyleChromesthesia();
    
    initializeInterfaceRenderTexture();

    handleWindowSizeChangeEvent();

    actionCenter_ = std::make_unique<ActionCenter>();
    canvasManager_ = std::make_unique<CanvasManager>(getInterfaceContext());
    midiManager_ = std::make_unique<MidiManager>(getMidiContext());
    systemState_.initializationError = midiManager_->initialization();
    
    if(systemState_.initializationError != program_states::System::InitializationError::None){
#ifndef PLATFORM_WEB
        SetWindowMinSize(
            constants::application_window::ErrorWindowWidth, 
            constants::application_window::ErrorWindowHeight
        );
        SetWindowSize(
            constants::application_window::ErrorWindowWidth, 
            constants::application_window::ErrorWindowHeight
        );
#endif
        
        return;
    }
    
    playbackManager_ = std::make_unique<PlaybackManager>(getMidiContext());
    serializer_ = std::make_unique<Serializer>();
    // enumerator_ = std::make_unique<Enumerator>();

    // stagedObserver_ = actionCenter_.getStagedObserver();
    systemState_.project.data = actionCenter_->getStagedObserver();

    actionCenter_->onReadOnlyViolation = [this](){
        interfaceState_.toolbar.readOnlyBlinkTimer = constants::toolbar::ReadOnlyBlinkDurationInSeconds;
    };
}

#ifdef PLATFORM_WEB
// TODO
namespace web_file_loading{
    std::string pendingLoadFile{};
    bool isIdbfsReady{false};
}

extern "C"{
    void EMSCRIPTEN_KEEPALIVE onWebFileLoaded(const char* filename){
        web_file_loading::pendingLoadFile = filename;
    }
    void EMSCRIPTEN_KEEPALIVE onIdbfsReady(){
        web_file_loading::isIdbfsReady = true;
    }
}
#endif

void MainWindow::update(){
    if(systemState_.initializationError != program_states::System::InitializationError::None){
        return;
    }

    handleEvents();

#ifdef PLATFORM_WEB
    if(web_file_loading::isIdbfsReady){
        web_file_loading::isIdbfsReady = false;
        std::string filenameToLoad{constants::serializer::DefaultFilename};
        if(FileExists(constants::serializer::LastProjectFile)){
            char *lastProjectContent{LoadFileText(constants::serializer::LastProjectFile)};
            if(lastProjectContent){
                filenameToLoad = std::string{lastProjectContent};
                UnloadFileText(lastProjectContent);
            }
        }
        std::string loadPath{std::string{constants::serializer::IdbfsMountPoint} + "/" + filenameToLoad};
        EM_ASM({ console.log("cpp received onIdbfsReady. Attempting to load: ", UTF8ToString($0)); }, loadPath.c_str());
        if(auto loadedData{serializer_->load(loadPath)}){

            EM_ASM({ console.log("Successfully loaded data from IDBFS"); });

            if(auto projectData{systemState_.project.data.lock()}){
                projectData->data.reset();
                loadProjectData(loadedData.value());
            }
        }else{

            EM_ASM({ console.log("failed to load data from IDBFS"); });
        }
    }
    if(!web_file_loading::pendingLoadFile.empty()){
        if(auto loadedData{serializer_->load("/" + web_file_loading::pendingLoadFile)}){
            if(auto projectData{systemState_.project.data.lock()}){
                projectData->data.reset();
                loadProjectData(loadedData.value());
            }
        }
        web_file_loading::pendingLoadFile.clear();
    }
#endif

    // midiManager_->update();

    if(!interfaceState_.prompts.isAnyPromptVisible()){
        canvasManager_->update(*actionCenter_, *midiManager_);
    }
    
    playbackManager_->update(*midiManager_);

    if(auto projectData{utilities::projectDataWithPagesFrom(systemState_)}){
        projectData->transient.currentPageNumber = systemState_.project.currentPage;
        projectData->transient.selectedChannelListViewIndex = interfaceState_.sidebar.selectedChannelListViewIndex;
    }

    updateWindowTitle();
}

void MainWindow::draw(){
    auto context{getInterfaceContext()};

    if(systemState_.initializationError != program_states::System::InitializationError::None){
        BeginDrawing(); {
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

            const char *errorMessage{
                systemState_.initializationError == program_states::System::InitializationError::SoundFontMissing
                    ? constants::application_window::ErrorMessageSoundFontMissing
                    : constants::application_window::ErrorMessageSoundFontLoadError
            };

            Rectangle bounds{
                static_cast<float>(constants::application_window::ErrorWindowMargin),
                static_cast<float>(constants::application_window::ErrorWindowMargin),
                static_cast<float>(GetScreenWidth() - constants::application_window::ErrorWindowMargin * 2),
                static_cast<float>(GetScreenHeight() - constants::application_window::ErrorWindowMargin * 2)
            };
            
            const int previousTextAlignment{GuiGetStyle(TEXTBOX, TEXT_ALIGNMENT)};
            GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
            
            GuiTextBox(bounds, const_cast<char*>(errorMessage), 0, false);
            
            GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, previousTextAlignment);
        } EndDrawing();
        return;
    }

    BeginTextureMode(interfaceRenderTexture_); {
        // ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        ClearBackground(BLANK);
            const bool shouldDisableToolbar(machineState_.isPlaying);
            if(shouldDisableToolbar) GuiDisable();

            const bool isPromptActive{interfaceState_.prompts.isAnyPromptVisible()};
            if(isPromptActive) GuiLock();
            interface::Toolbar::draw(context);
            if(isPromptActive) GuiUnlock();
            if(shouldDisableToolbar) GuiEnable();

            const bool shouldBlockUnderlyingUi{isPromptActive};
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
