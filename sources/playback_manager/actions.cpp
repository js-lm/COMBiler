#include "playback_manager.hpp"

#include <array>
#include <variant>

#include "constants.hpp"
#include "aliases.hpp"

#include "debug_utilities.hpp"

#include "utilities/project_utilities.hpp"

void PlaybackManager::setupPlayback(MidiManager &midiManager){
    threadState_.internalMachine.reset();
    threadState_.internalMachine.playheadIndex = 0;
    timeSinceLastNote_ = .0f;
    needsToPlayCurrentNote_ = true;
    // if(isHardRest) timeSinceLastNote_ = .0f;

    const auto projectDataSlot{context_.system.project.data.lock()};
    if(!projectDataSlot) return;
    const auto &projectData{projectDataSlot->data};

    // int currentPageIndex{context_.system.project.currentPage - 1};

    const auto machineState{utilities::machineStateAt(
        *projectData,
        threadState_.internalCurrentPage,
        threadState_.internalMachine.playheadIndex
    )};

    threadState_.internalMachine.tempo = machineState.tempo;
    threadState_.internalMachine.instruments = machineState.instruments;
    threadState_.internalMachine.volumes = machineState.volumes;
    threadState_.internalMachine.articulations = machineState.articulations;

    midiManager.setTempo(command::Tempo{threadState_.internalMachine.tempo});

    for(size_t channel{0}; channel < constants::project_data::NumberOfInstrumentChannels; channel++){
        midiManager.setInstrument(channelIndexToChannelTarget(channel), threadState_.internalMachine.instruments[channel]);
        midiManager.setVolume(channelIndexToChannelTarget(channel), threadState_.internalMachine.volumes[channel]);
        midiManager.setArticulation(channelIndexToChannelTarget(channel), threadState_.internalMachine.articulations[channel]);
    }

    // while(context_.system.project.currentPage > 1 && )

// interfaceState_.navigationBar.requestedPageNumber

}

void PlaybackManager::playCurrentNote(MidiManager &midiManager){
    // TODO: duplications
    const auto projectDataSlot{context_.system.project.data.lock()};
    if(!projectDataSlot) return;
    const auto &projectData{projectDataSlot->data};

    int currentPageIndex{threadState_.internalCurrentPage - 1};

    const auto &currentPage{projectData->pages[currentPageIndex]};
    const int currentPageNoteCount{currentPage.noteInThisPage.value_or(projectData->metadata.notePerPage)};

    auto &machine{threadState_.internalMachine};
    auto &playheadIndex{machine.playheadIndex};

    if(currentPage.commandChannel[playheadIndex]){
        const auto &commandToken{currentPage.commandChannel[playheadIndex].value()};

        std::optional<command::Command> commandOptional{projectData->commandPalette.get(commandToken)};

        if(commandOptional.has_value()){
            std::visit([&](const auto &command){
                using Type = std::decay_t<decltype(command)>;

                if constexpr(std::is_same_v<command::Tempo, Type>){
                    midiManager.setTempo(command);
                    machine.tempo = command.tempo;
                }else if constexpr(std::is_same_v<command::Volume, Type>){
                    midiManager.setVolume(command.target, command.volume);
                    if(command.target == command::Target::All_Channels){
                        machine.volumes.fill(command.volume);
                    }else{
                        machine.volumes[static_cast<size_t>(command.target) - 1] = command.volume;
                    }
                }else if constexpr(std::is_same_v<command::Articulation, Type>){
                    midiManager.setArticulation(command.target, command.articulation);
                    if(command.target == command::Target::All_Channels){
                        machine.articulations.fill(command.articulation);
                    }else{
                        machine.articulations[static_cast<size_t>(command.target) - 1] = command.articulation;
                    }
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

}

void PlaybackManager::advancePlayhead(MidiManager &midiManager){
    const auto projectDataSlot{context_.system.project.data.lock()};
    if(!projectDataSlot) return;
    const auto &projectData{projectDataSlot->data};

    int currentPageIndex{threadState_.internalCurrentPage - 1};
    const auto &currentPage{projectData->pages[currentPageIndex]};
    const int currentPageNoteCount{currentPage.noteInThisPage.value_or(projectData->metadata.notePerPage)};

    auto &machine{threadState_.internalMachine};
    auto &playheadIndex{machine.playheadIndex};

    if(playheadIndex >= currentPageNoteCount - 1){
        if(threadState_.internalIsPageRepeatEnabled){
            stopPlayback(midiManager);
            setupPlayback(midiManager);
        }else{
            if(threadState_.internalCurrentPage >= projectData->pages.size()){
                machine.isPlaying = false;
                stopPlayback(midiManager);
            }else{
                threadState_.internalCurrentPage = threadState_.internalCurrentPage + 1;
                needsToPlayCurrentNote_ = true;
            }

            playheadIndex = 0;
        }
    }else{
        playheadIndex++;
        needsToPlayCurrentNote_ = true;
    }
}

void PlaybackManager::releaseStaccatoNotes(MidiManager &midiManager){
    for(int channel{0}; channel < constants::project_data::NumberOfInstrumentChannels; channel++){
        if(threadState_.internalMachine.articulations[channel] == units::machine::Articulation::Staccato){
            
            const auto targetChannel{channelIndexToChannelTarget(static_cast<units::midi::SoundFontChannel>(channel))};
            const auto activeNotes{threadState_.internalMachine.activeNotes[channel].toVector()};
            
            for(const auto &activeNote : activeNotes){
                midiManager.noteOff(targetChannel, activeNote);
                threadState_.internalMachine.activeNotes[channel].remove(activeNote);
            }
        }
    }
}