#include "main_window.hpp"

#include "constants.hpp"

#include <raylib.h>
#include <raygui.h>

#include "interface/interface.hpp"

#include "program_states/context.hpp"

#include <cstdlib>
#include <cstring>
#include <chrono>
#include <thread>
#include <vector>
#include <string>

#include "resources/styles/chromesthesia.h"
#include "resources/icons/iconset.h"

#include "debug_utilities.hpp"

#include <tsf.h>

void DEBUG_sf2(){
    InitAudioDevice();

    std::string soundFontPath{GetApplicationDirectory()};
    soundFontPath += "soundfonts/gm_bank.sf2";
    
    if(!FileExists(soundFontPath.c_str())){
        DEBUG_PRINT("soundFontPath: {}", soundFontPath.c_str());
    }

    tsf *soundFont{tsf_load_filename(soundFontPath.c_str())};

    const int sampleRate{10000}; // 44100
    const int outputChannelCount{2};
    const int totalFrameCount{sampleRate * 2};
    const int noteOnFrameCount{sampleRate / 2};
    const int releaseFrameCount{totalFrameCount - noteOnFrameCount};
    const int midiChannel{0};
    const int midiNoteNumber{60};

    tsf_set_output(soundFont, TSF_STEREO_INTERLEAVED, sampleRate, .0f);
    tsf_note_on(soundFont, midiChannel, midiNoteNumber, .9f);

    std::vector<int16_t> interleavedSamples(static_cast<size_t>(totalFrameCount * outputChannelCount));
    tsf_render_short(soundFont, interleavedSamples.data(), noteOnFrameCount, 0);

    tsf_note_off(soundFont, midiChannel, midiNoteNumber);
    tsf_render_short(
        soundFont,
        interleavedSamples.data() + (noteOnFrameCount * outputChannelCount),
        releaseFrameCount,
        0
    );

    Wave soundWave{};
    soundWave.frameCount = totalFrameCount;
    soundWave.sampleRate = sampleRate;
    soundWave.sampleSize = 16;
    soundWave.channels = outputChannelCount;
    soundWave.data = interleavedSamples.data();

    Sound renderedSound{LoadSoundFromWave(soundWave)};
    if(renderedSound.stream.buffer){
        PlaySound(renderedSound);
        
        while(IsSoundPlaying(renderedSound)){
            DEBUG_SLEEP_MS(10);
        }
        UnloadSound(renderedSound);
    }

    // AudioStream outputAudioStream{LoadAudioStream(sampleRate, 16, outputChannelCount)};
    // if(outputAudioStream.buffer){
    //     const int streamChunkFrameCount{1024};
    //     int streamedFrameOffset{0};

    //     PlayAudioStream(outputAudioStream);

    //     while(streamedFrameOffset < totalFrameCount){
    //         if(IsAudioStreamProcessed(outputAudioStream)){
    //             const int remainingFrameCount{totalFrameCount - streamedFrameOffset};
    //             const int currentChunkFrameCount{
    //                 remainingFrameCount < streamChunkFrameCount
    //                     ? remainingFrameCount
    //                     : streamChunkFrameCount
    //             };

    //             UpdateAudioStream(
    //                 outputAudioStream,
    //                 interleavedSamples.data() + (streamedFrameOffset * outputChannelCount),
    //                 currentChunkFrameCount
    //             );

    //             streamedFrameOffset += currentChunkFrameCount;
    //         }else{
    //             DEBUG_SLEEP_MS(1);
    //         }
    //     }

    //     const int playbackDurationMs{(totalFrameCount * 1000) / sampleRate};
    //     DEBUG_SLEEP_MS(playbackDurationMs + 50);

    //     StopAudioStream(outputAudioStream);
    //     UnloadAudioStream(outputAudioStream);
    // }

    tsf_close(soundFont);
    CloseAudioDevice();
}

int MainWindow::run(){

    // DEBUG_sf2();
    // return 0;

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