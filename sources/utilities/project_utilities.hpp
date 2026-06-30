#pragma once

#include <algorithm>
#include <memory>
#include <optional>

#include "constants.hpp"

#include "program_states/project_data.hpp"
#include "program_states/system.hpp"
#include "program_states/machine.hpp"

namespace utilities{

    inline std::shared_ptr<program_states::ProjectData> projectDataFrom(const program_states::System &systemState){
        const auto stagedSlot{systemState.project.data.lock()};
        if(!stagedSlot || !stagedSlot->data) return {};

        return stagedSlot->data;
    }

    inline std::shared_ptr<program_states::ProjectData> projectDataWithPagesFrom(const program_states::System &systemState){
        const auto projectData{projectDataFrom(systemState)};
        if(!projectData || projectData->pages.empty()) return {};

        return projectData;
    }

    inline int currentPageIndexFrom(const program_states::ProjectData &projectData, int currentPageNumber){
        if(projectData.pages.empty()) return 0;

        return std::clamp(
            currentPageNumber - 1,
            0, static_cast<int>(projectData.pages.size()) - 1
        );
    }

    inline int currentPageNoteCountFrom(const program_states::ProjectData &projectData, int currentPageNumber){
        if(projectData.pages.empty()) return constants::project_data::MinimumNotePerPage;

        const int currentPageIndex{currentPageIndexFrom(projectData, currentPageNumber)};
        return std::clamp(
            projectData.pages[currentPageIndex].noteInThisPage.value_or(projectData.metadata.notePerPage),
            constants::project_data::MinimumNotePerPage, constants::project_data::MaximumNotePerPage
        );
    }

    inline bool hasSpecificInstrumentChannelSelection(int selectedChannelListViewIndex){
        return selectedChannelListViewIndex >= 1
            && selectedChannelListViewIndex <= constants::project_data::NumberOfInstrumentChannels;
    }

    inline std::optional<int> selectedInstrumentChannelIndex(int selectedChannelListViewIndex){
        if(!hasSpecificInstrumentChannelSelection(selectedChannelListViewIndex)) return std::nullopt;

        return selectedChannelListViewIndex - 1;
    }

    inline int selectedChannelListIndexFromInstrumentChannelIndex(int instrumentChannelIndex){
        return instrumentChannelIndex + 1;
    }

    inline program_states::ProjectData::Page &pageByNumber(program_states::ProjectData &projectData, int pageNumber){
        return projectData.pages[pageNumber - 1];
    }
    inline const program_states::ProjectData::Page &pageByNumber(const program_states::ProjectData &projectData, int pageNumber){
        return projectData.pages[pageNumber - 1];
    }

    // inline void applyTransientSelection(
    //     program_states::ProjectData::Transient &transient,
    //     int pageNumber,
    //     int instrumentChannelIndex
    // ){
    //     transient.currentPageNumber = pageNumber;
    //     transient.selectedChannelListViewIndex = selectedChannelListIndexFromInstrumentChannelIndex(instrumentChannelIndex);
    // }


    inline program_states::Machine machineStateAt(
        const program_states::ProjectData &projectData,
        int pageNumber,
        int noteIndex
    ){
        // TODO: I want to be able to resume from the middle, don't do a hard reset?
        program_states::Machine machine{};

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

        int currentPageIndex{currentPageIndexFrom(projectData, pageNumber)};

        do{

            const auto &currentPage{projectData.pages[currentPageIndex]};
            
            const int currentPageNoteCount{currentPage.noteInThisPage.value_or(projectData.metadata.notePerPage)};

            int startHead{
                (currentPageIndex == pageNumber - 1) 
                    ? noteIndex
                    : currentPageNoteCount
            };

            for(int head{startHead}; head --> 0;){

                for(units::midi::SoundFontChannel channel{0}; channel < constants::project_data::NumberOfInstrumentChannels; channel++){
                    if(!isInstrumentFound[channel]){
                        const auto &cell{currentPage.instrumentChannels[channel][head]};
                        if(cell.has_value()){
                            if(const auto *instrument{std::get_if<music_data::Instrument>(&*cell)}){
                                machine.instruments[channel] = *instrument;
                                // midiManager.setInstrument(channelIndexToChannelTarget(channel), *instrument);
                                isInstrumentFound[channel] = true;
                            }
                        }
                    }
                }

                const auto &commandOptional{currentPage.commandChannel[head]};
                if(commandOptional.has_value()){
                    const auto &commandToken{*commandOptional};

                    std::optional<command::Command> commandOptional{projectData.commandPalette.get(commandToken)};

                    if(commandOptional.has_value()){

                        std::visit([&](const auto &commandToken){
                            using Type = std::decay_t<decltype(commandToken)>;

                            if constexpr(std::is_same_v<command::Tempo, Type>){
                                if(!isTempoFound){
                                    machine.tempo = commandToken.tempo;
                                    // midiManager.setTempo(commandToken);
                                    isTempoFound = true;
                                }
                            }else if constexpr(std::is_same_v<command::Volume, Type>){
                                auto [volume, target]{commandToken};

                                auto assignVolume{[&](size_t channel){
                                    if(!isVolumeFound[channel]){
                                        machine.volumes[channel] = units::machine::Volume{volume};
                                        // midiManager.setVolume(channelIndexToChannelTarget(channel), volume);
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
                                        machine.articulations[channel] = articulation;
                                        // midiManager.setArticulation(channelIndexToChannelTarget(channel), articulation);
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

                        }, commandOptional.value());

                    }
                }

                if(isAllFound()) return machine;
            }

            currentPageIndex--;

        }while(currentPageIndex >= 0);

        return machine;
    }

} // namespace utilities
