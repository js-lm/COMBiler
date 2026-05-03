#include "serializer.hpp"

#include "external/tinyfiledialogs/tinyfiledialogs.h"

#include "constants.hpp"

#include "debug_utilities.hpp"

void Serializer::save(const program_states::ProjectData &data, bool saveAsNewFile){

    const std::string pattern{std::string{"*."} + constants::serializer::SaveFileExtension};
    const char *filterPattern[1]{pattern.c_str()};

    DEBUG_PRINT("currentFilename_: [{}]", currentFilename_);

    if(currentFilename_.empty() || saveAsNewFile){
        const char *saveFilename{tinyfd_saveFileDialog(
            constants::serializer::SaveDialogTitle,
            currentFilename_.empty() ? constants::serializer::DefaultFilename : currentFilename_.c_str(),
            1, filterPattern,
            constants::serializer::FilterDescription
        )};

        if(!saveFilename){
            DEBUG_PRINT("Cancel saving");
            return;
        }

        currentFilename_ = GetFileName(saveFilename);
        currentWorkingDirectory_ = GetDirectoryPath(saveFilename);
    }
    
    std::string saveFilePath{currentWorkingDirectory_ + "/" + currentFilename_};

    DEBUG_PRINT("PWD: {}\nFilename: {}\nFinal: {}", currentWorkingDirectory_, currentFilename_, saveFilePath);

    SaveFileText(saveFilePath.c_str(), const_cast<char*>(toString(data).c_str()));

}

void Serializer::load(){


    
}