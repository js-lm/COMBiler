#include "serializer.hpp"

#include <sstream>
#include <string>
#include <variant>

const std::string Serializer::toString(const program_states::ProjectData &data) const{
    std::stringstream savefile;

    savefile << "# VERSION " << constants::serializer::Version << "\n\n";

    savefile << "[METADATA]\n";
    savefile << "PAGES " << data.metadata.numberOfPages << "\n";
    savefile << "NOTES_PER_PAGE " << data.metadata.notePerPage << "\n\n";

    savefile << "[CONSTANTS]\n";
    auto constantsList{data.commandPalette.getList()};
    for(int constantIndex{0}; constantIndex < constantsList.size(); constantIndex++){
        const auto &[name, commandOptional]{constantsList[constantIndex]};

        if(commandOptional.has_value()){
            savefile << constantIndex << " " << emitCommand(*commandOptional) << " " << name << "\n";
        }
    }

    savefile << "\n";

    for(int pageIndex{0}; pageIndex < data.pages.size(); pageIndex++){
        const auto &page{data.pages[pageIndex]};

        savefile << "[PAGE " << (pageIndex + 1) << "]\n";
        
        int notesInPage{page.noteInThisPage.value_or(data.metadata.notePerPage)};
        savefile << "NOTES " << notesInPage << "\n";

        for(int channel{0}; channel < constants::project_data::NumberOfInstrumentChannels; channel++){
            savefile << "CH" << (channel + 1);
            for(int noteIndex{0}; noteIndex < notesInPage; noteIndex++){
                savefile << " " << emitChannelData(page.instrumentChannels[channel][noteIndex]);
            }
            savefile << "\n";
        }

        savefile << "CMD";
        for(int noteIndex{0}; noteIndex < notesInPage; noteIndex++){
            savefile << " " << emitCommandToken(page.commandChannel[noteIndex]);
        }
        savefile << "\n\n";
    }

    return savefile.str();
}

std::string Serializer::emitChannelData(const std::optional<music_data::InstrumentChannelData> &channelData) const{

    if(!channelData.has_value()) return "-";
    

    return std::visit([](const auto &data)->std::string{
        using Type = std::decay_t<decltype(data)>;

        if constexpr(std::is_same_v<Type, music_data::Note>){
            return std::to_string(static_cast<int>(data));
        }else if constexpr(std::is_same_v<Type, music_data::Instrument>){
            return "I(" + std::to_string(static_cast<int>(data)) + ")";
        }

        return "-";
    }, channelData.value());
}

std::string Serializer::emitCommand(const command::Command &command) const{
    return std::visit([](const auto &data)->std::string{
        using Type = std::decay_t<decltype(data)>;

        if constexpr(std::is_same_v<Type, command::Tempo>){
            return "T(" + std::to_string(data.tempo) + ")";
        }else if constexpr(std::is_same_v<Type, command::Volume>){

            return "V(" + std::to_string(static_cast<int>(data.target)) + "," + std::to_string(data.volume) + ")";
        
        }else if constexpr(std::is_same_v<Type, command::Articulation>){
            return "A(" + std::to_string(static_cast<int>(data.target)) + "," + std::to_string(static_cast<int>(data.articulation)) + ")";
        }

        return "-";
    }, command);

}

std::string Serializer::emitCommandToken(const std::optional<command::CommandToken> &commandToken) const{
    if(!commandToken.has_value()) return "-";

    return std::visit([this](const auto &data)->std::string{
        using Type = std::decay_t<decltype(data)>;

        if constexpr(std::is_same_v<Type, command::Command>){
            return emitCommand(data);
        }else if constexpr(std::is_same_v<Type, command::ConstantIndex>){
            return "C(" + std::to_string(static_cast<int>(data)) + ")";
        }

        return "-";
    }, commandToken.value());
}