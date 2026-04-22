#include "main_window.hpp"

#include "constants.hpp"

#include <algorithm>
#include <cmath>

#include <raymath.h>

#include "utilities/project_utilities.hpp"

float MainWindow::calculateMaximumScaleFactor() const{
    using namespace constants::application_window;

    return std::max(
        MinimumScaleFactor,
        std::min(
            static_cast<float>(GetScreenWidth()) / static_cast<float>(MinimalWidth),
            static_cast<float>(GetScreenHeight()) / static_cast<float>(MinimalHeight)
        )
    );
}

float MainWindow::calculateScaleFactor() const{
    using namespace constants::application_window;

    const auto &window{systemState_.window};
    const float dynamicMaximumScaleFactor{calculateMaximumScaleFactor()};

    float scaleFactor{window.isAlwaysScalingToMaximum ? dynamicMaximumScaleFactor : window.scaleFactor};
    scaleFactor = std::clamp(scaleFactor, MinimumScaleFactor, dynamicMaximumScaleFactor);

    if(window.isIntegerScaling){
        scaleFactor = std::clamp(
            std::round(scaleFactor),
            MinimumScaleFactor,
            std::max(MinimumScaleFactor, std::floor(dynamicMaximumScaleFactor))
        );
    }

    return scaleFactor;
}

bool MainWindow::isScaleFactorIntegral() const{
    const auto &window{systemState_.window};

    return window.isIntegerScaling 
        || FloatEquals(window.scaleFactor, std::round(window.scaleFactor));
}

void MainWindow::applyTextureFilter(){
    SetTextureFilter(
        interfaceRenderTexture_.texture,
        isScaleFactorIntegral() ? TEXTURE_FILTER_POINT : TEXTURE_FILTER_BILINEAR
    );
}

program_states::InterfaceContext MainWindow::getInterfaceContext(){
    return program_states::InterfaceContext{
        .system     {systemState_},
        .layout     {layoutState_},
        .interface  {interfaceState_}
    };
}
program_states::MidiContext MainWindow::getMidiContext(){
    return program_states::MidiContext{
        .machine{machineState_}
    };
}

program_states::ReadOnlyContext MainWindow::getReadOnlyContext(){
    return program_states::ReadOnlyContext{
        .system     {systemState_},
        .layout     {layoutState_},
        .interface  {interfaceState_}
    };
}

void MainWindow::applyProjectTransientNavigationState(){
    const auto projectData{utilities::projectDataWithPagesFrom(systemState_)};
    if(!projectData) return;
    const auto &transient{projectData->transient};

    systemState_.project.currentPage = transient.currentPageNumber;
    interfaceState_.sidebar.selectedChannelListViewIndex = transient.selectedChannelListViewIndex;

    interfaceState_.noteCanvas.isGridLayoutDirty = true;
}