#pragma once

#include <raylib.h>

#include "aliases.hpp"

#include "utilities/enum_map.hpp"

#include <unordered_set>
#include <vector>

namespace constants::prompts{

    enum class CommandPrompt{
        Tempo,
        Volume,
        Articulation,
        Change_Instrument
    };

    constexpr int CommandToolCount{3};
    constexpr int OpenCommandPromptButtonToolIndex{3};

    constexpr int toIndex(CommandPrompt selectedPrompt){
        return static_cast<int>(selectedPrompt);
    }

    inline CommandPrompt commandPromptFromIndex(int commandPromptIndex){
        switch(commandPromptIndex){
        case toIndex(CommandPrompt::Tempo): return CommandPrompt::Tempo;
        case toIndex(CommandPrompt::Volume): return CommandPrompt::Volume;
        default: return CommandPrompt::Articulation;
        }
    }

    constexpr int TempoPercentageMinimumValue{2};
    constexpr int TempoPercentageMaximumValue{100};
    constexpr float OverlayDimAlpha{.35f};


    // inline const std::unordered_set<music_data::InstrumentFamily>

    inline const std::vector<music_data::Instrument> getInstruments(music_data::InstrumentFamily family){
        using namespace music_data;

        switch(family){
        case InstrumentFamily::Keyboard: return {
            Instrument::Grand_Piano,
            Instrument::Electric_Piano,
            Instrument::Drawbar_Organ,
            Instrument::Church_Organ
        };

        case InstrumentFamily::Percussion: return {
            Instrument::Music_Box,
            Instrument::Marimba,
            Instrument::Tubular_Bells,
            Instrument::Glockenspiel,
            Instrument::Vibraphone,
            Instrument::Kalimba
        };

        case InstrumentFamily::Woodwind: return {
            Instrument::Flute,
            Instrument::Oboe,
            Instrument::Clarinet,
            Instrument::Bassoon
        };

        case InstrumentFamily::String: return {
            Instrument::Violin,
            Instrument::Viola,
            Instrument::Cello,
            Instrument::Contrabass,
            Instrument::Orchestral_Harp
        };

        case InstrumentFamily::Brass: return {
            Instrument::Trumpet,
            Instrument::French_Horn,
            Instrument::Trombone,
            Instrument::Tuba
        };

        case InstrumentFamily::Harmony: return {
            Instrument::String_Ensemble,
            Instrument::Pizzicato_Strings,
            Instrument::Choir_Aahs
        };

        case InstrumentFamily::Guitar: return {
            Instrument::Nylon_Acoustic_Guitar,
            Instrument::Steel_Acoustic_Guitar,
            Instrument::Electric_Guitar,
            Instrument::Overdriven_Guitar
        };

        case InstrumentFamily::Bass: return {
            Instrument::Acoustic_Bass,
            Instrument::Finger_Bass,
            Instrument::Fretless_Bass,
            Instrument::Synthesizer_Bass
        };

        case InstrumentFamily::Reed: return {
            Instrument::Alto_Saxophone,
            Instrument::Tenor_Saxophone
        };

        case InstrumentFamily::Synthesizer: return {
            Instrument::Lead_1_Square,
            Instrument::Lead_2_Sawtooth,
            Instrument::Lead_3_Calliope,
            Instrument::Pad_1_Fantasia,
            Instrument::Pad_2_Warm,
            Instrument::Synthesizer_Strings
        };

        case InstrumentFamily::World: return {
            Instrument::Shamisen,
            Instrument::Steel_Drum,
            Instrument::Sitar,
            Instrument::Accordion_French,
            Instrument::Harmonica,
            Instrument::Banjo,
            Instrument::Pan_Flute
        };

        case InstrumentFamily::Drum_Sets: return {
            Instrument::Drum_Sets
        };
        }

        return {};
    }

    namespace constants_manager{
        enum class Type{
            None,
            Tempo,
            Volume,
            Articulation
        };

        constexpr int TypeCount{4};

        constexpr int toIndex(Type selectedType){
            return static_cast<int>(selectedType);
        }

        inline Type typeFromIndex(int typeIndex){
            switch(typeIndex){
            case toIndex(Type::Tempo): return Type::Tempo;
            case toIndex(Type::Volume): return Type::Volume;
            case toIndex(Type::Articulation): return Type::Articulation;
            default: return Type::None;
            }
        }

        constexpr int ConstantNameTextMaximumLength{30};
        constexpr int SwapIndexTextMaximumLength{3};

    } // namespace constants_manager

} // namespace constants::prompts
