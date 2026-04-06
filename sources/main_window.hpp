#pragma once

#include "program_states/interface.hpp"
#include "program_states/system.hpp"
#include "program_states/layout.hpp"

#include "action_center/action_center.hpp"
#include "canvas_manager/canvas_manager.hpp"

#include "program_states/context.hpp"

#include <raylib.h>

#include <memory>

class MainWindow{
private:
    program_states::System      systemState_{};
    program_states::Interface   interfaceState_{};
    program_states::Layout      layoutState_{};

private:
    std::unique_ptr<ActionCenter> actionCenter_;
    std::unique_ptr<CanvasManager> canvasManager_;

private:
    RenderTexture2D interfaceRenderTexture_{};

public:
    MainWindow() = default;
    ~MainWindow() = default;

    int run();

public:
    program_states::InterfaceContext getInterfaceContext();
    program_states::ReadOnlyContext getReadOnlyContext();

private:

    void initialize();

    void update();
    void draw();

private:
    void initializeInterfaceRenderTexture();
    void unloadInterfaceRenderTexture();

    
private:
    void handleEvents();
    
    void handleWindowSizeChangeEvent();
    void handleKeyboardEvent();


private:
    void handleButtonPressEvents();
    void handlePageChangeButtonsEvents();
    void handleToolbarButtonsEvents();

private:
    void applyProjectTransientNavigationState();

private:
    float calculateMaximumScaleFactor() const;
    float calculateScaleFactor() const;

    bool isScaleFactorIntegral() const;
    void applyTextureFilter();
};