#include "main_window.hpp"

#include "constants.hpp"

#include <algorithm>
#include <cmath>

#include <raymath.h>

#include "utilities/project_utilities.hpp"
#include "utilities/project_utilities.hpp"

#include "constants/serializer.hpp"

#include <string>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <fstream>

#ifdef PLATFORM_WEB
#include <emscripten.h>
#endif

// float MainWindow::calculateMaximumScaleFactor() const{
//     using namespace constants::application_window;

//     return std::max(
//         MinimumScaleFactor,
//         std::min(
//             static_cast<float>(GetScreenWidth()) / static_cast<float>(MinimalWidth),
//             static_cast<float>(GetScreenHeight()) / static_cast<float>(MinimalHeight)
//         )
//     );
// }

// float MainWindow::calculateScaleFactor() const{
//     using namespace constants::application_window;

//     const auto &window{systemState_.window};
//     const float dynamicMaximumScaleFactor{calculateMaximumScaleFactor()};

//     float scaleFactor{window.isAlwaysScalingToMaximum ? dynamicMaximumScaleFactor : window.scaleFactor};
//     scaleFactor = std::clamp(scaleFactor, MinimumScaleFactor, dynamicMaximumScaleFactor);

//     if(window.isIntegerScaling){
//         scaleFactor = std::clamp(
//             std::round(scaleFactor),
//             MinimumScaleFactor,
//             std::max(MinimumScaleFactor, std::floor(dynamicMaximumScaleFactor))
//         );
//     }

//     return scaleFactor;
// }

// bool MainWindow::isScaleFactorIntegral() const{
//     const auto &window{systemState_.window};
//
//     return window.isIntegerScaling 
//         || FloatEquals(window.scaleFactor, std::round(window.scaleFactor));
// }

float MainWindow::calculateMaximumScaleFactor(){
    using namespace constants::application_window;

    return std::max(
        MinimumScaleFactor,
        std::min(
            static_cast<float>(GetScreenWidth()) / static_cast<float>(MinimalWidth),
            static_cast<float>(GetScreenHeight()) / static_cast<float>(MinimalHeight)
        )
    );
}

float MainWindow::calculateScaleFactor(const program_states::System &systemState){
    using namespace constants::application_window;

    const auto &window{systemState.window};
    const float dynamicMaximumScaleFactor{calculateMaximumScaleFactor()};

    float scaleFactor{window.isAlwaysScalingToMaximum ? dynamicMaximumScaleFactor : window.scaleFactor};
    if(std::isnan(scaleFactor)) scaleFactor = 1.0f;
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

bool MainWindow::isScaleFactorIntegral(const program_states::System &systemState){
    const auto &window{systemState.window};

    return window.isIntegerScaling 
        || FloatEquals(window.scaleFactor, std::round(window.scaleFactor));
}

void MainWindow::applyTextureFilter(){
    SetTextureFilter(
        interfaceRenderTexture_.texture,
        isScaleFactorIntegral(systemState_) ? TEXTURE_FILTER_POINT : TEXTURE_FILTER_BILINEAR
    );
}

program_states::InterfaceContext MainWindow::getInterfaceContext(){
    return program_states::InterfaceContext{
        .system     {systemState_},
        .layout     {layoutState_},
        .interface  {interfaceState_},
        .machine    {machineState_}
    };
}
program_states::MidiContext MainWindow::getMidiContext(){
    return program_states::MidiContext{
        .system     {systemState_},
        .interface  {interfaceState_},
        .machine    {machineState_}
    };
}

program_states::ReadOnlyContext MainWindow::getReadOnlyContext(){
    return program_states::ReadOnlyContext{
        .system     {systemState_},
        .machine    {machineState_},
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

void MainWindow::updateWindowTitle(){
    static std::string previousTitle{};

    std::string newWindowTitle{constants::application_window::Title};
// #ifndef PLATFORM_WEB
    if(serializer_){
        if(const auto filename{serializer_->getCurrentFilename()}; !filename.empty()){
            newWindowTitle += " - " + filename;
        }
    }
// #endif
    if(actionCenter_ && actionCenter_->isUnsaved()){
        newWindowTitle += "*";
    }

    if(newWindowTitle != previousTitle){
        SetWindowTitle(newWindowTitle.c_str());
        previousTitle = newWindowTitle;
    }
}

void MainWindow::loadProjectData(program_states::ProjectData &data){
    machineState_.isPlaying = false;
    machineState_.shouldResetPlayback = true;
    machineState_.playheadIndex = 0;
    
    systemState_.project.currentPage = 1;
    interfaceState_.navigationBar.requestedPageNumber = 1;
    
    actionCenter_->loadFile(data);
}

void MainWindow::saveConfiguration(){
    std::string directoryPath{
#ifdef PLATFORM_WEB
        std::string{constants::serializer::IdbfsMountPoint}
#else
#if defined(__linux__)
        std::string{std::getenv("XDG_CONFIG_HOME") ? std::getenv("XDG_CONFIG_HOME") : (std::string{std::getenv("HOME")} + "/.config")} + "/COMBiler"
#elif defined(__APPLE__)
        std::string{std::getenv("HOME")} + "/Library/Application Support/COMBiler"
#elif defined(_WIN32)
        std::string{std::getenv("APPDATA")} + "\\COMBiler"
#else
        std::string{GetWorkingDirectory()} + "/config"
#endif
#endif
    };
    
#ifndef PLATFORM_WEB
    if(!DirectoryExists(directoryPath.c_str())){
#if defined(_WIN32)
        std::string command{"mkdir \"" + directoryPath + "\""};
#else
        std::string command{"mkdir -p \"" + directoryPath + "\""};
#endif
        std::system(command.c_str());
    }
#endif

    std::string filePath{directoryPath + "/config"};
    std::stringstream stringStream{};
    stringStream << "scaleFactor=" << systemState_.window.scaleFactor << "\n";
    stringStream << "isAlwaysScalingToMaximum=" << (systemState_.window.isAlwaysScalingToMaximum ? 1 : 0) << "\n";
    stringStream << "isIntegerScaling=" << (systemState_.window.isIntegerScaling ? 1 : 0) << "\n";
    stringStream << "masterVolume=" << systemState_.audio.masterVolume << "\n";
    stringStream << "limitFpsTo60=" << (systemState_.performance.limitFpsTo60 ? 1 : 0) << "\n";
    stringStream << "idleDuringEventWaiting=" << (systemState_.performance.idleDuringEventWaiting ? 1 : 0) << "\n";
    
    SaveFileText(filePath.c_str(), const_cast<char*>(stringStream.str().c_str()));

#ifdef PLATFORM_WEB
    EM_ASM({
        FS.syncfs(false, function(error){
            if(error){
                console.error('Error syncing to IDBFS: ', error);
            }else{
                console.log('successfully synced config to IDBFS');
            }
        });
    });
#endif
}

void MainWindow::loadConfiguration(){
    std::string directoryPath{
#ifdef PLATFORM_WEB
        std::string{constants::serializer::IdbfsMountPoint}
#else
#if defined(__linux__)
        std::string{std::getenv("XDG_CONFIG_HOME") ? std::getenv("XDG_CONFIG_HOME") : (std::string{std::getenv("HOME")} + "/.config")} + "/COMBiler"
#elif defined(__APPLE__)
        std::string{std::getenv("HOME")} + "/Library/Application Support/COMBiler"
#elif defined(_WIN32)
        std::string{std::getenv("APPDATA")} + "\\COMBiler"
#else
        std::string{GetWorkingDirectory()} + "/config"
#endif
#endif
    };
    std::string filePath{directoryPath + "/config"};
    if(!FileExists(filePath.c_str())) return;

    char *fileData{LoadFileText(filePath.c_str())};
    if(!fileData) return;

    std::string fileContent{fileData};
    UnloadFileText(fileData);

    std::stringstream stringStream{fileContent};
    std::string currentLine{};
    
    while(std::getline(stringStream, currentLine)){
        if(currentLine.empty()) continue;
        size_t delimiterPosition{currentLine.find('=')};
        if(delimiterPosition == std::string::npos) continue;

        std::string configurationKey{currentLine.substr(0, delimiterPosition)};
        std::string configurationValue{currentLine.substr(delimiterPosition + 1)};

        try{
            if(configurationKey == "scaleFactor"){
                systemState_.window.scaleFactor = std::stof(configurationValue);
                if(std::isnan(systemState_.window.scaleFactor)) systemState_.window.scaleFactor = 1.0f;
            }else if(configurationKey == "isAlwaysScalingToMaximum"){
                systemState_.window.isAlwaysScalingToMaximum = (std::stoi(configurationValue) != 0);
            }else if(configurationKey == "isIntegerScaling"){
                systemState_.window.isIntegerScaling = (std::stoi(configurationValue) != 0);
            }else if(configurationKey == "masterVolume"){
                systemState_.audio.masterVolume = std::stof(configurationValue);
            }else if(configurationKey == "limitFpsTo60"){
                systemState_.performance.limitFpsTo60 = (std::stoi(configurationValue) != 0);
            }else if(configurationKey == "idleDuringEventWaiting"){
                systemState_.performance.idleDuringEventWaiting = (std::stoi(configurationValue) != 0);
            }
        }catch(...){
            // ignore parsing errors for individual keys
        }
    }

    systemState_.window.isLayoutDirty = true;
}
