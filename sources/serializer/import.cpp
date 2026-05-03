#include "serializer.hpp"

#include <string>
#include <sstream>

program_states::ProjectData Serializer::toProjectData(const std::string &data) const{

    program_states::ProjectData projectData{};
    std::istringstream  dataStream{data};
    std::string         currentLine{};

    enum class Section{
        None,
        Metadata,
        Constants,
        Page
    };

    Section currentSection{Section::None};
    int currentPageIndex{-1};

    while(std::getline(dataStream, currentLine)){
        if(currentLine.empty() || currentLine.find("# VERSION") == 0) continue;

        if(currentLine == "[METADATA]"){
            currentSection = Section::Metadata;
            continue;
        }else if(currentLine == "[CONSTANTS]"){
            currentSection = Section::Constants;
            continue;
        }else if(currentLine.find("[PAGE ") == 0){
            currentSection = Section::Page;
            projectData.pages.push_back(program_states::ProjectData::Page{});
            currentPageIndex++;
            
            continue;
        }

        std::istringstream lineStream{currentLine};
        std::string tokenString{};
        
        if(!(lineStream >> tokenString)) continue;

        if(currentSection == Section::Metadata){
            if(tokenString == "PAGES"){
                lineStream >> projectData.metadata.numberOfPages;
            }else if(tokenString == "NOTES_PER_PAGE"){
                lineStream >> projectData.metadata.notePerPage;
            }
        }else if(currentSection == Section::Constants){
            int constantIndexValue{std::stoi(tokenString)};
            std::string commandTokenString{};
            
            lineStream >> commandTokenString;
            
            std::string constantName{};
            std::string temporaryName{};
            
            while(lineStream >> temporaryName){
                if(!constantName.empty()) constantName += " ";
                
                constantName += temporaryName;
            }
            
            projectData.commandPalette.set(static_cast<command::ConstantIndex>(constantIndexValue), constantName, parseCommand(commandTokenString));
        }else if(currentSection == Section::Page && currentPageIndex >= 0){
            program_states::ProjectData::Page &currentPage{projectData.pages[currentPageIndex]};

            if(tokenString == "NOTES"){
                int noteCount{};
                lineStream >> noteCount;
                currentPage.noteInThisPage = noteCount;
            }else if(tokenString.find("CH") == 0){
                int channelIndex{tokenString[2] - '1'};
                
                if(channelIndex >= 0 && channelIndex < constants::project_data::NumberOfInstrumentChannels){
                    int noteIndex{0};
                    std::string dataTokenString{};
                    
                    while(lineStream >> dataTokenString && noteIndex < constants::project_data::MaximumNotePerPage){
                        currentPage.instrumentChannels[channelIndex][noteIndex] = parseChannelData(dataTokenString);
                        noteIndex++;
                    }
                }
            }else if(tokenString == "CMD"){
                int noteIndex{0};
                std::string dataTokenString{};
                
                while(lineStream >> dataTokenString && noteIndex < constants::project_data::MaximumNotePerPage){
                    currentPage.commandChannel[noteIndex] = parseCommandToken(dataTokenString);
                    noteIndex++;
                }
            }
        }
    }

    return projectData;
}

std::optional<music_data::InstrumentChannelData> Serializer::parseChannelData(const std::string &data) const{
    if(data == "-") return std::nullopt;

    if(data.find("I(") == 0 && data.back() == ')'){
        int instrumentValue{std::stoi(data.substr(2, data.length() - 3))};
        
        return static_cast<music_data::Instrument>(instrumentValue);
    }

    int noteValue{std::stoi(data)};
    
    return static_cast<music_data::Note>(noteValue);
}

std::optional<command::CommandToken> Serializer::parseCommandToken(const std::string &data) const{
    if(data == "-") return std::nullopt;

    if(data.find("C(") == 0 && data.back() == ')'){
        int constantIndexValue{std::stoi(data.substr(2, data.length() - 3))};
        
        return static_cast<command::ConstantIndex>(constantIndexValue);
    }

    return parseCommand(data);
}

command::Command Serializer::parseCommand(const std::string &data) const{
    if(data.find("T(") == 0 && data.back() == ')'){
        int tempoValue{std::stoi(data.substr(2, data.length() - 3))};
        
        return command::Tempo{static_cast<units::machine::Tempo>(tempoValue)};
    }

    if(data.find("V(") == 0 && data.back() == ')'){
        std::string innerData{data.substr(2, data.length() - 3)};
        size_t commaPosition{innerData.find(',')};
        
        int targetValue{std::stoi(innerData.substr(0, commaPosition))};
        int volumeValue{std::stoi(innerData.substr(commaPosition + 1))};
        
        return command::Volume{
            static_cast<units::machine::Volume>(volumeValue),
            static_cast<command::Target>(targetValue)
        };
    }

    if(data.find("A(") == 0 && data.back() == ')'){
        std::string innerData{data.substr(2, data.length() - 3)};
        size_t commaPosition{innerData.find(',')};
        
        int targetValue{std::stoi(innerData.substr(0, commaPosition))};
        int articulationValue{std::stoi(innerData.substr(commaPosition + 1))};
        
        return command::Articulation{
            static_cast<units::machine::Articulation>(articulationValue),
            static_cast<command::Target>(targetValue)
        };
    }

    return command::Tempo{constants::midi::DefaultTempo};

}