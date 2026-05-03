#include "serializer.hpp"

#include "external/tinyfiledialogs/tinyfiledialogs.h"

#include "constants.hpp"

#include "debug_utilities.hpp"

void Serializer::save(const program_states::ProjectData &data, bool saveAsNewFile){

    const std::string pattern{std::string{"*."} + constants::serializer::SaveFileExtension};
    const char *filterPattern[1]{pattern.c_str()};

    DEBUG_PRINT("currentFilename_: [{}]", currentFilename_);

    if(currentFilename_.empty() || saveAsNewFile){
        const char *saveFilePath{tinyfd_saveFileDialog(
            constants::serializer::SaveDialogTitle,
            currentFilename_.empty() ? constants::serializer::DefaultFilename : currentFilename_.c_str(),
            1, filterPattern,
            constants::serializer::FilterDescription
        )};

        if(!saveFilePath){
            DEBUG_PRINT("Cancel saving");
            return;
        }

        currentFilename_ = GetFileName(saveFilePath);
        currentWorkingDirectory_ = GetDirectoryPath(saveFilePath);
    }
    
    std::string saveFilePath{currentWorkingDirectory_ + "/" + currentFilename_};

    DEBUG_PRINT("PWD: {}\nFilename: {}\nFinal: {}", currentWorkingDirectory_, currentFilename_, saveFilePath);

    SaveFileText(saveFilePath.c_str(), const_cast<char*>(toString(data).c_str()));

}

std::optional<program_states::ProjectData> Serializer::load(){

    std::string currentSaveFilePath{currentWorkingDirectory_ + "/" + currentFilename_};

    const std::string pattern{std::string{"*."} + constants::serializer::SaveFileExtension};
    const char *filterPattern[1]{pattern.c_str()};

    const char *saveFilePath{tinyfd_openFileDialog(
        constants::serializer::LoadDialogTitle,
        currentSaveFilePath.c_str(),
        1, filterPattern,
        constants::serializer::FilterDescription,
        0
    )};

    DEBUG_PRINT("Loading {}", saveFilePath);

    if(!FileExists(saveFilePath)){
        DEBUG_PRINT("Cancel loading");
        return std::nullopt;
    }

    currentFilename_ = GetFileName(saveFilePath);
    currentWorkingDirectory_ = GetDirectoryPath(saveFilePath);

    DEBUG_PRINT("PWD: {}\nFilename: {}\nOriginal: {}", currentWorkingDirectory_, currentFilename_, saveFilePath);

    char *loadedSaveFile{LoadFileText(const_cast<char*>(saveFilePath))};

    DEBUG_PRINT("{}", loadedSaveFile);
    
    return toProjectData(loadedSaveFile);

}