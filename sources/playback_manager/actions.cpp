#include "playback_manager.hpp"

#include <array>
#include <variant>

#include "constants.hpp"
#include "aliases.h"

void PlaybackManager::setupPlayback(){

    const auto projectDataSlot{context_.system.project.data.lock()};
    if(!projectDataSlot) return;
    const auto &projectData{projectDataSlot->data};

    bool isTempoFound{false};
    std::array<bool, constants::project_data::NumberOfInstrumentChannels> isInstrumentFound{};
    std::array<bool, constants::project_data::NumberOfInstrumentChannels> isVolumeFound{};
    std::array<bool, constants::project_data::NumberOfInstrumentChannels> isArticulationFound{};

    auto isAllFound{[&](){
        if(!isTempoFound) return false;
        for(bool found : isInstrumentFound){ if(!found) return false;}
        for(bool found : isVolumeFound){ if(!found) return false;}
        for(bool found : isArticulationFound){ if(!found) return false;}
        return true;
    }};

    int currentPageNumber{context_.system.project.currentPage - 1};

    do{

        const auto &currentPage{projectData->pages[currentPageNumber]};
        
        const int currentPageNoteCount{currentPage.noteInThisPage.value_or(projectData->metadata.notePerPage)};

        int startHead{
            (currentPageNumber == context_.system.project.currentPage - 1) 
                ? context_.machine.playheadIndex
                : currentPageNoteCount
        };

        for(int head{startHead}; head --> 0;){

            for(size_t channel{0}; channel < constants::project_data::NumberOfInstrumentChannels; channel++){
                if(!isInstrumentFound[channel]){
                    const auto &cell{currentPage.instrumentChannels[channel][head]};
                    if(cell.has_value()){
                        if(const auto *instrument{std::get_if<music_data::Instrument>(&*cell)}){
                            context_.machine.instruments[channel] = *instrument;
                            isInstrumentFound[channel] = true;
                        }
                    }
                }
            }

            const auto &commandOptional{currentPage.commandChannel[head]};
            if(commandOptional.has_value()){
                const auto &commandToken{*commandOptional};

                // TODO: constants
                if(const auto *command{std::get_if<command::Command>(&commandToken)}){

                    std::visit([&](const auto &commandToken){
                        using Type = std::decay_t<decltype(commandToken)>;

                        if constexpr(std::is_same_v<command::Tempo, Type>){
                            if(!isTempoFound){
                                context_.machine.tempo = commandToken;
                                isTempoFound = true;
                            }
                        }else if constexpr(std::is_same_v<command::Volume, Type>){
                            auto [level, target]{commandToken};

                            auto assignVolume{[&](size_t channel){
                                if(!isVolumeFound[channel]){
                                    context_.machine.volumes[channel] = units::machine::Volume{level};
                                    isVolumeFound[channel] = true;
                                }
                            }};
                            
                            switch(target){
                            case command::Target::All_Channels: {
                                for(size_t channel{0}; channel < constants::project_data::NumberOfInstrumentChannels; channel++){
                                    assignVolume(channel);
                                }
                            } break;
                            case command::Target::Channel_1: assignVolume(0); break;
                            case command::Target::Channel_2: assignVolume(1); break;
                            case command::Target::Channel_3: assignVolume(2); break;
                            case command::Target::Channel_4: assignVolume(3); break;
                            }


                        }else if constexpr(std::is_same_v<command::Articulation, Type>){
                            auto [articulation, target]{commandToken};
                            auto assignArticulation{[&](size_t channel){
                                if(!isArticulationFound[channel]){
                                    context_.machine.articulations[channel] = articulation;
                                    isArticulationFound[channel] = true;
                                }
                            }};

                            switch(target){
                            case command::Target::All_Channels: {
                                for(size_t channel{0}; channel < constants::project_data::NumberOfInstrumentChannels; channel++){
                                    assignArticulation(channel);
                                }
                            } break;
                            case command::Target::Channel_1: assignArticulation(0); break;
                            case command::Target::Channel_2: assignArticulation(1); break;
                            case command::Target::Channel_3: assignArticulation(2); break;
                            case command::Target::Channel_4: assignArticulation(3); break;
                            }
                        }

                    }, *command); //

                }
            }

            if(isAllFound()) return;
        }

        currentPageNumber--;

    }while(currentPageNumber >= 0);

    // while(context_.system.project.currentPage > 1 && )

// interfaceState_.navigationBar.requestedPageNumber

}