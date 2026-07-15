#include "playback_manager.hpp"

void PlaybackManager::updateNoteState(
    units::midi::SoundFontChannel channel, 
    std::optional<music_data::Note> currentNote, 
    MidiManager &midiManager
){
    const auto articulation{threadState_.internalMachine.articulations[channel]};
    const auto targetChannel{static_cast<command::Target>(channel + 1)};

    if(currentNote.has_value()){
        const music_data::Note note{currentNote.value()};

        switch(articulation){
        case units::machine::Articulation::Infinite: {
            const bool isCurrentlyPlaying{threadState_.internalMachine.activeNotes[channel].remove(note)};
            if(isCurrentlyPlaying){
                threadState_.internalMachine.activeNotes[channel].add(note);
                midiManager.noteOff(targetChannel, note);
            }else{
                midiManager.noteOn(targetChannel, note);
                threadState_.internalMachine.activeNotes[channel].add(note);
            }
        } break;
        
        case units::machine::Articulation::Sustain:
        case units::machine::Articulation::Legato: {
            const bool isCurrentlyPlaying{threadState_.internalMachine.activeNotes[channel].remove(note)};
            if(isCurrentlyPlaying){
                threadState_.internalMachine.activeNotes[channel].add(note);
            }else{
                const auto activeNotes{threadState_.internalMachine.activeNotes[channel].toVector()};
                for(const auto &activeNote : activeNotes){
                    midiManager.noteOff(targetChannel, activeNote);
                }
                
                midiManager.noteOn(targetChannel, note);
                threadState_.internalMachine.activeNotes[channel].clear();
                threadState_.internalMachine.activeNotes[channel].add(note);
            }
        } break;

        case units::machine::Articulation::Normal:
        case units::machine::Articulation::Staccato:
        default: {
            const auto activeNotes{threadState_.internalMachine.activeNotes[channel].toVector()};
            for(const auto &activeNote : activeNotes){
                midiManager.noteOff(targetChannel, activeNote);
            }
            
            midiManager.noteOn(targetChannel, note);

            threadState_.internalMachine.activeNotes[channel].clear();
            threadState_.internalMachine.activeNotes[channel].add(note);
        } break;
        }
    }else{
        switch(articulation){
        case units::machine::Articulation::Staccato:
            // TODO: staccato


        case units::machine::Articulation::Normal:
        case units::machine::Articulation::Legato: {
            const auto activeNotes{threadState_.internalMachine.activeNotes[channel].toVector()};
            for(const auto &activeNote : activeNotes){
                midiManager.noteOff(targetChannel, activeNote);
            }
            threadState_.internalMachine.activeNotes[channel].clear();
        } break;
        case units::machine::Articulation::Sustain: {}
        case units::machine::Articulation::Infinite:
        default: break;
        }
    }

}