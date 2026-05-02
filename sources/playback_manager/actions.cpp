#include "playback_manager.hpp"

#include <array>
#include <variant>

#include "constants.hpp"
#include "aliases.h"

#include "debug_utilities.hpp"

void PlaybackManager::setupPlayback(MidiManager &midiManager){
    context_.machine.reset();
    context_.machine.playheadIndex = 0;
    timeSinceLastNote_ = .0f;
    // if(isHardRest) timeSinceLastNote_ = .0f;

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

    int currentPageIndex{context_.system.project.currentPage - 1};

    do{

        const auto &currentPage{projectData->pages[currentPageIndex]};
        
        const int currentPageNoteCount{currentPage.noteInThisPage.value_or(projectData->metadata.notePerPage)};

        int startHead{
            (currentPageIndex == context_.system.project.currentPage - 1) 
                ? context_.machine.playheadIndex
                : currentPageNoteCount
        };

        for(int head{startHead}; head --> 0;){

            for(units::midi::SoundFontChannel channel{0}; channel < constants::project_data::NumberOfInstrumentChannels; channel++){
                if(!isInstrumentFound[channel]){
                    const auto &cell{currentPage.instrumentChannels[channel][head]};
                    if(cell.has_value()){
                        if(const auto *instrument{std::get_if<music_data::Instrument>(&*cell)}){
                            context_.machine.instruments[channel] = *instrument;
                            midiManager.setInstrument(channelIndexToChannelTarget(channel), *instrument);
                            isInstrumentFound[channel] = true;
                        }
                    }
                }
            }

            const auto &commandOptional{currentPage.commandChannel[head]};
            if(commandOptional.has_value()){
                const auto &commandToken{*commandOptional};

                std::optional<command::Command> commandOptional{projectData->commandPalette.get(commandToken)};

                if(commandOptional.has_value()){

                    std::visit([&](const auto &commandToken){
                        using Type = std::decay_t<decltype(commandToken)>;

                        if constexpr(std::is_same_v<command::Tempo, Type>){
                            if(!isTempoFound){
                                context_.machine.tempo = commandToken.tempo;
                                midiManager.setTempo(commandToken);
                                isTempoFound = true;
                            }
                        }else if constexpr(std::is_same_v<command::Volume, Type>){
                            auto [volume, target]{commandToken};

                            auto assignVolume{[&](size_t channel){
                                if(!isVolumeFound[channel]){
                                    context_.machine.volumes[channel] = units::machine::Volume{volume};
                                    midiManager.setVolume(channelIndexToChannelTarget(channel), volume);
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
                                    midiManager.setArticulation(channelIndexToChannelTarget(channel), articulation);
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

            if(isAllFound()) return;
        }

        currentPageIndex--;

    }while(currentPageIndex >= 0);


    if(!isTempoFound) midiManager.setTempo(command::Tempo{context_.machine.tempo});

    for(size_t channel{0}; channel < constants::project_data::NumberOfInstrumentChannels; channel++){
        if(!isInstrumentFound[channel]){
            midiManager.setInstrument(channelIndexToChannelTarget(channel), context_.machine.instruments[channel]);
        }
        if(!isVolumeFound[channel]){
            midiManager.setVolume(channelIndexToChannelTarget(channel), context_.machine.volumes[channel]);
        }
        if(!isArticulationFound[channel]){
            midiManager.setArticulation(channelIndexToChannelTarget(channel), context_.machine.articulations[channel]);
        }
    }

    // while(context_.system.project.currentPage > 1 && )

// interfaceState_.navigationBar.requestedPageNumber

}

void PlaybackManager::nextNote(MidiManager &midiManager){
    // TODO: duplications
    const auto projectDataSlot{context_.system.project.data.lock()};
    if(!projectDataSlot) return;
    const auto &projectData{projectDataSlot->data};

    int currentPageIndex{context_.system.project.currentPage - 1};

    const auto &currentPage{projectData->pages[currentPageIndex]};
    const int currentPageNoteCount{currentPage.noteInThisPage.value_or(projectData->metadata.notePerPage)};

    auto &machine{context_.machine};
    auto &playheadIndex{machine.playheadIndex};

    if(currentPage.commandChannel[playheadIndex]){
        const auto &commandToken{currentPage.commandChannel[playheadIndex].value()};

        std::optional<command::Command> commandOptional{projectData->commandPalette.get(commandToken)};

        if(commandOptional.has_value()){
            std::visit([&](const auto &command){
                using Type = std::decay_t<decltype(command)>;

                if constexpr(std::is_same_v<command::Tempo, Type>){
                    midiManager.setTempo(command);
                }else if constexpr(std::is_same_v<command::Volume, Type>){
                    midiManager.setVolume(command.target, command.volume);
                }else if constexpr(std::is_same_v<command::Articulation, Type>){
                    midiManager.setArticulation(command.target, command.articulation);
                }
            }, commandOptional.value());
        }
    }

    for(units::midi::SoundFontChannel channel{0}; channel < constants::project_data::NumberOfInstrumentChannels; channel++){

        std::optional<music_data::Note> currentNote{};

        if(currentPage.instrumentChannels[channel][playheadIndex]){
            // const auto &instrumentChannelData{currentPage.instrumentChannels[channel][playheadIndex].value()};
            
            // if(const auto *data{std::get_if<music_data::Instrument>(&instrumentChannelData)}){
            std::visit([&](const auto &noteData){
                using Type = std::decay_t<decltype(noteData)>;

                if constexpr(std::is_same_v<music_data::Note, Type>){

                    // midiManager.noteOn(channelIndexToChannelTarget(channel), noteData);
                    // updateNoteState(channel, noteData);
                    currentNote = noteData;

                }else if constexpr(std::is_same_v<music_data::Instrument, Type>){
                    midiManager.setInstrument(channelIndexToChannelTarget(channel), noteData);
                }

            }, currentPage.instrumentChannels[channel][playheadIndex].value());
            // }else{

            // }
        }

        updateNoteState(channel, currentNote, midiManager);

    }

    if(playheadIndex >= currentPageNoteCount - 1){
        if(context_.interface.navigationBar.isPageRepeatEnabled){
            // float previousTime{timeSinceLastNote_};
            setupPlayback(midiManager);
            // timeSinceLastNote_ = previousTime; 
        }else{
            if(context_.system.project.currentPage >= projectData->pages.size()){
                machine.isPlaying = false;
            }else{
                context_.interface.navigationBar.requestedPageNumber = context_.system.project.currentPage + 1;
            }

            playheadIndex = 0;
        }
    }else{
        playheadIndex++;
    }
}