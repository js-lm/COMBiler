#include "serializer.hpp"

#include <external/tinyfiledialogs/tinyfiledialogs.h>

#include "constants.hpp"

#include "debug_utilities.hpp"

void Serializer::save(const program_states::ProjectData &data, bool saveAsNewFile){

    const std::string pattern{std::string{"*."} + constants::serializer::SaveFileExtension};
    const char *filterPattern[1]{pattern.c_str()};

    DEBUG_PRINT("currentFilename_: [{}]", currentFilename_);

    if(currentFilename_.empty() 
    || saveAsNewFile 
    || !FileExists(std::string{currentWorkingDirectory_ + "/" + currentFilename_}.c_str())
    ){
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

    if(!saveFilePath){
        DEBUG_PRINT("Cancel loading");
        return std::nullopt;
    }

    return load(std::string{saveFilePath});
}

std::optional<program_states::ProjectData> Serializer::load(const std::string &filePath){
    DEBUG_PRINT("Loading {}", filePath);

    if(!FileExists(filePath.c_str())){
        DEBUG_PRINT("File does not exist: {}", filePath);
        return std::nullopt;
    }

    currentFilename_ = GetFileName(filePath.c_str());
    currentWorkingDirectory_ = GetDirectoryPath(filePath.c_str());

    DEBUG_PRINT("PWD: {}\nFilename: {}\nOriginal: {}", currentWorkingDirectory_, currentFilename_, filePath);

    char *loadedSaveFile{LoadFileText(filePath.c_str())};

    DEBUG_PRINT("{}", loadedSaveFile);
    
    return toProjectData(loadedSaveFile);

}