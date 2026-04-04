#pragma once

#include "utilities/enum_map.hpp"

#include "aliases.hpp"
#include "constants.hpp"

#include <array>

namespace constants{

    // constexpr utilities::EnumMap<music_data::Instrument, const char*> InstrumentIconMappings{
    constexpr utilities::EnumMap<music_data::Instrument, int> InstrumentIconMappings{
    // Keyboards
        {music_data::Instrument::Grand_Piano,       128},
        {music_data::Instrument::Electric_Piano,    129},
        {music_data::Instrument::Drawbar_Organ,     130},
        {music_data::Instrument::Church_Organ,      131},

    // Percussion
        {music_data::Instrument::Music_Box,     136},
        {music_data::Instrument::Marimba,       137},
        {music_data::Instrument::Tubular_Bells, 138},
        {music_data::Instrument::Glockenspiel,  139},
        {music_data::Instrument::Vibraphone,    140},
        {music_data::Instrument::Kalimba,       141},

    // Woodwinds
        {music_data::Instrument::Flute,     144},
        {music_data::Instrument::Piccolo,   145},
        {music_data::Instrument::Oboe,      146},
        {music_data::Instrument::Clarinet,  147},
        {music_data::Instrument::Bassoon,   148},

    // Strings
        {music_data::Instrument::Violin,            152},
        {music_data::Instrument::Viola,             153},
        {music_data::Instrument::Cello,             154},
        {music_data::Instrument::Contrabass,        155},
        {music_data::Instrument::Pizzicato_Strings, 156},
        {music_data::Instrument::Orchestral_Harp,   157},

    // Brass
        {music_data::Instrument::Trumpet,       160},
        {music_data::Instrument::French_Horn,   161},
        {music_data::Instrument::Trombone,      162},
        {music_data::Instrument::Tuba,          163},

    // Harmony
        {music_data::Instrument::String_Ensemble,   168},
        {music_data::Instrument::Choir_Aahs,        169},

    // Guitars
        {music_data::Instrument::Nylon_Acoustic_Guitar, 176},
        {music_data::Instrument::Steel_Acoustic_Guitar, 177},
        {music_data::Instrument::Electric_Guitar,       178},
        {music_data::Instrument::Overdriven_Guitar,     179},

    // Basses
        {music_data::Instrument::Acoustic_Bass,     184},
        {music_data::Instrument::Finger_Bass,       185},
        {music_data::Instrument::Fretless_Bass,     186},
        {music_data::Instrument::Synthesizer_Bass,  187},

    // Reeds
        {music_data::Instrument::Alto_Saxophone,    192},
        {music_data::Instrument::Tenor_Saxophone,   193},

    // Synthesizers
        {music_data::Instrument::Lead_1_Square,         200},
        {music_data::Instrument::Lead_2_Sawtooth,       201},
        {music_data::Instrument::Lead_3_Calliope,       202},
        {music_data::Instrument::Pad_1_Fantasia,        203},
        {music_data::Instrument::Pad_2_Warm,            204},
        {music_data::Instrument::Synthesizer_Strings,   205},

    // Others
        {music_data::Instrument::Shamisen,          208},
        {music_data::Instrument::Steel_Drum,        209},
        {music_data::Instrument::Sitar,             210},
        {music_data::Instrument::Accordion_French,  211},
        {music_data::Instrument::Harmonica,         212},
        {music_data::Instrument::Banjo,             213},
        {music_data::Instrument::Pan_Flute,         214}
    };

    constexpr std::array<music_data::Instrument, constants::project_data::NumberOfInstrumentChannels> DefaultInstrumentByChannel{
        music_data::Instrument::Marimba,
        music_data::Instrument::Violin,
        music_data::Instrument::Finger_Bass,
        music_data::Instrument::Shamisen
    };
    


} // namespace constants