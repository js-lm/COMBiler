#include "playback_manager.hpp"

#include <array>
#include <variant>

#include "constants.hpp"
#include "aliases.h"

#include "debug_utilities.hpp"

#include "utilities/project_utilities.hpp"

void PlaybackManager::setupPlayback(MidiManager &midiManager){
    context_.machine.reset();
    context_.machine.playheadIndex = 0;
    timeSinceLastNote_ = .0f;
    // if(isHardRest) timeSinceLastNote_ = .0f;

    const auto projectDataSlot{context_.system.project.data.lock()};
    if(!projectDataSlot) return;
    const auto &projectData{projectDataSlot->data};

    // int currentPageIndex{context_.system.project.currentPage - 1};

    const auto machineState{utilities::machineStateAt(
        *projectData,
        context_.system.project.currentPage,
        context_.machine.playheadIndex
    )};

    context_.machine.tempo = machineState.tempo;
    context_.machine.instruments = machineState.instruments;
    context_.machine.volumes = machineState.volumes;
    context_.machine.articulations = machineState.articulations;

    midiManager.setTempo(command::Tempo{context_.machine.tempo});

    for(size_t channel{0}; channel < constants::project_data::NumberOfInstrumentChannels; channel++){
        midiManager.setInstrument(channelIndexToChannelTarget(channel), context_.machine.instruments[channel]);
        midiManager.setVolume(channelIndexToChannelTarget(channel), context_.machine.volumes[channel]);
        midiManager.setArticulation(channelIndexToChannelTarget(channel), context_.machine.articulations[channel]);
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

        std::optional<music_data::Instrument> switchInstrumentTo;

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
                    // midiManager.setInstrument(channelIndexToChannelTarget(channel), noteData);
                    switchInstrumentTo = noteData;
                }

            }, currentPage.instrumentChannels[channel][playheadIndex].value());
            // }else{

            // }
        }

        updateNoteState(channel, currentNote, midiManager);

        if(switchInstrumentTo) midiManager.setInstrument(channelIndexToChannelTarget(channel), switchInstrumentTo.value());

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