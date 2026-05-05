#pragma once

#include "aliases.hpp"

#include "constants.hpp"

#include <array>
#include <vector>
#include <unordered_set>
#include <optional>

#include "utilities/enum_set.hpp"
#include "utilities/static_set.hpp"

namespace program_states{

    struct Machine{
        bool isPlaying{false};
        bool shouldResetPlayback{false};
        int playheadIndex{0}; // relative to the page

        units::machine::Tempo tempo{};
        
        std::array<music_data::Instrument, constants::project_data::NumberOfInstrumentChannels> instruments{};
        // std::array<utilities::EnumSet<music_data::Note>, constants::project_data::NumberOfInstrumentChannels> onNotes{};
        
        std::array<units::machine::Volume, constants::project_data::NumberOfInstrumentChannels> volumes{};
        std::array<units::machine::Articulation, constants::project_data::NumberOfInstrumentChannels> articulations{};

        // struct ActiveNotes{
        // private:
        //     // std::array<std::optional<music_data::Note>, constants::midi::MaximumSimultaneousNote> notes_{};
        //     std::array<music_data::Note, constants::midi::MaximumSimultaneousNote> notes_{};
        //     size_t activeCount{0};

        // public:
        //     void addNote(const music_data::Note note);
        //     void removeNote(const music_data::Note note);

        //     std::vector<music_data::Note> getAllActiveNotes() const;

        //     void size() const;
        //     void clear();
        // };

        using ActiveNotes = utilities::StaticSet<music_data::Note, constants::midi::MaximumSimultaneousNotePerInstrument>;

        std::array<ActiveNotes, constants::project_data::NumberOfInstrumentChannels> activeNotes{};

    public:
        Machine(){ reset();}
        ~Machine() = default;

        void reset(){
            // instruments = constants::midi::DefaultInstruments;

            tempo = constants::midi::DefaultTempo;
            instruments.fill(constants::midi::DefaultInstrument);
            volumes.fill(constants::midi::DefaultVolume);
            articulations.fill(constants::midi::DefaultArticulation);

            for(auto &notesSet : activeNotes){
                notesSet.clear();
            }
        }
    };

} // namespace program_states