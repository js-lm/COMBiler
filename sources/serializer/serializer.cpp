#include "serializer.hpp"

#ifdef PLATFORM_WEB
#include <emscripten.h>
#else
#include <external/tinyfiledialogs/tinyfiledialogs.h>
#endif

#include "constants.hpp"

#include "debug_utilities.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

void Serializer::save(program_states::ProjectData &data, bool saveAsNewFile){

    const auto now{std::chrono::system_clock::now()};
    const std::time_t time{std::chrono::system_clock::to_time_t(now)};
    std::stringstream timeStringStream{};
    timeStringStream << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    const std::string currentTimeString{timeStringStream.str()};

    if(data.metadata.creationDate[0] == '\0'){
        strncpy(data.metadata.creationDate, currentTimeString.c_str(), constants::project_data::MetadataDateMaximumLength);
        data.metadata.creationDate[constants::project_data::MetadataDateMaximumLength] = '\0';
    }
    
    strncpy(data.metadata.modificationDate, currentTimeString.c_str(), constants::project_data::MetadataDateMaximumLength);
    data.metadata.modificationDate[constants::project_data::MetadataDateMaximumLength] = '\0';

    const std::string pattern{std::string{"*."} + constants::serializer::SaveFileExtension};
    const char *filterPattern[1]{pattern.c_str()};

    DEBUG_PRINT("currentFilename_: [{}]", currentFilename_);

#ifdef PLATFORM_WEB
    if(saveAsNewFile){
        const std::string content{toString(data)};
        const std::string filename{currentFilename_.empty() ? constants::serializer::DefaultFilename : currentFilename_};
        
        EM_ASM_({
            var contentString = UTF8ToString($0);
            var filenameString = UTF8ToString($1);
            var dataBlob = new Blob([contentString], {type: 'text/plain'});
            var objectUrl = URL.createObjectURL(dataBlob);
            var anchorElement = document.createElement('a');
            anchorElement.href = objectUrl;
            anchorElement.download = filenameString;
            document.body.appendChild(anchorElement);
            anchorElement.click();
            document.body.removeChild(anchorElement);
            URL.revokeObjectURL(objectUrl);
        }, content.c_str(), filename.c_str());

        return;
    }

    if(currentFilename_.empty()){
        currentFilename_ = constants::serializer::DefaultFilename;
    }
    
    currentWorkingDirectory_ = constants::serializer::IdbfsMountPoint;
    std::string saveFilePath{std::string{currentWorkingDirectory_} + "/" + currentFilename_};

    DEBUG_PRINT("PWD: {}\nFilename: {}\nFinal: {}", currentWorkingDirectory_, currentFilename_, saveFilePath);

    SaveFileText(saveFilePath.c_str(), const_cast<char*>(toString(data).c_str()));
    SaveFileText(constants::serializer::LastProjectFile, const_cast<char*>(currentFilename_.c_str()));
    
    EM_ASM({
        console.log("writing to IDBFS triggered from cpp");

        FS.syncfs(false, function(error){
            if(error){
                console.error('Error syncing to IDBFS: ', error);
            }else{
                console.log('successfully synced to IDBFS');
            }
        });
    });
#else
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
#endif
}

std::optional<program_states::ProjectData> Serializer::load(){

#ifdef PLATFORM_WEB
    const std::string extensionPattern{std::string{"."} + std::string{constants::serializer::SaveFileExtension}};
    EM_ASM_({
        var extensionType = UTF8ToString($0);

        var inputElement = document.createElement('input');
        inputElement.type = 'file';
        inputElement.accept = extensionType;
        inputElement.onchange = function(event){
            var selectedFile = event.target.files[0];
            if(!selectedFile){
                return;
            }
            var fileReader = new FileReader();
            fileReader.onload = function(loadEvent){
                var fileContent = new Uint8Array(loadEvent.target.result);
                FS.writeFile('/' + selectedFile.name, fileContent);
                Module.ccall('onWebFileLoaded', 'void', ['string'], [selectedFile.name]);
            };
            fileReader.readAsArrayBuffer(selectedFile);
        };
        inputElement.click();
    }, extensionPattern.c_str());

    return std::nullopt; // async load, handeld by callback
#else
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
#endif
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