#pragma once

#include "aliases.hpp"

#include "utilities/enum_map.hpp"

namespace constants::midi{

    constexpr float MaximumNotePerSecond{12.0f};
    constexpr float StaccatoReleaseDuration{.33f};
    // constexpr float MinimumStaccatoReleaseDuration{.2f};
    // constexpr float MaximumStaccatoReleaseDuration{.5f};
    // constexpr float StaccatoReleaseDurationDifference{
    //     MaximumStaccatoReleaseDuration - MinimumStaccatoReleaseDuration
    // };
    // // constexpr float StaccatoReleaseDurationTempoRatio{.5f};

    constexpr size_t MaximumSimultaneousNotePerInstrument{16}; // 64 max / 4 channels

    constexpr int DefaultOutputSampleRate{44100};
    constexpr int OutputSampleSizeInBits{16};
    constexpr int OutputChannelCount{2};
    constexpr int StreamChunkFrameCount{1024};
    constexpr int MaximumVoiceCount{256};
    constexpr float GlobalGainInDecibel{.0f};

    constexpr int DrumSoundFontChannel{9};
    constexpr int DrumSoundPresetNumber{0};

    constexpr const char *SoundFontPath{"soundfonts/gm_bank.sf2"};

    constexpr utilities::EnumMap<music_data::Instrument, units::midi::SoundPresetID> InstrumentsMap{
        // Keyboards
        {music_data::Instrument::Grand_Piano, 0},
        {music_data::Instrument::Electric_Piano, 4},
        {music_data::Instrument::Drawbar_Organ, 16},
        {music_data::Instrument::Church_Organ, 19},

        // Percussion
        {music_data::Instrument::Music_Box, 10},
        {music_data::Instrument::Marimba, 12},
        {music_data::Instrument::Tubular_Bells, 14},
        {music_data::Instrument::Glockenspiel, 9},
        {music_data::Instrument::Vibraphone, 11},
        {music_data::Instrument::Kalimba, 108},

        // Woodwinds
        {music_data::Instrument::Flute, 73},
        {music_data::Instrument::Oboe, 68},
        {music_data::Instrument::Clarinet, 71},
        {music_data::Instrument::Bassoon, 70},

        // Strings
        {music_data::Instrument::Violin, 40},
        {music_data::Instrument::Viola, 41},
        {music_data::Instrument::Cello, 42},
        {music_data::Instrument::Contrabass, 43},
        {music_data::Instrument::Pizzicato_Strings, 45},
        {music_data::Instrument::Orchestral_Harp, 46},

        // Brass
        {music_data::Instrument::Trumpet, 56},
        {music_data::Instrument::French_Horn, 60},
        {music_data::Instrument::Trombone, 57},
        {music_data::Instrument::Tuba, 58},

        // Harmony
        {music_data::Instrument::String_Ensemble, 48},
        {music_data::Instrument::Choir_Aahs, 52},

        // Guitars
        {music_data::Instrument::Nylon_Acoustic_Guitar, 24},
        {music_data::Instrument::Steel_Acoustic_Guitar, 25},
        {music_data::Instrument::Electric_Guitar, 27},
        {music_data::Instrument::Overdriven_Guitar, 29},

        // Basses
        {music_data::Instrument::Acoustic_Bass, 32},
        {music_data::Instrument::Finger_Bass, 33},
        {music_data::Instrument::Fretless_Bass, 35},
        {music_data::Instrument::Synthesizer_Bass, 38},

        // Reeds
        {music_data::Instrument::Alto_Saxophone, 65},
        {music_data::Instrument::Tenor_Saxophone, 66},

        // Synthesizers
        {music_data::Instrument::Lead_1_Square, 80},
        {music_data::Instrument::Lead_2_Sawtooth, 81},
        {music_data::Instrument::Lead_3_Calliope, 82},
        {music_data::Instrument::Pad_1_Fantasia, 88},
        {music_data::Instrument::Pad_2_Warm, 89},
        {music_data::Instrument::Synthesizer_Strings, 50},

        // World
        {music_data::Instrument::Shamisen, 106},
        {music_data::Instrument::Steel_Drum, 114},
        {music_data::Instrument::Sitar, 104},
        {music_data::Instrument::Accordion_French, 21},
        {music_data::Instrument::Harmonica, 22},
        {music_data::Instrument::Banjo, 105},
        {music_data::Instrument::Pan_Flute, 75},

        // Drum Sets 
        {music_data::Instrument::Drum_Sets, -1} // special: channel 10
    };

    // constexpr std::array<music_data::Instrument, constants::project_data::NumberOfInstrumentChannels> DefaultInstruments{
    //     music_data::Instrument::Marimba,
    //     music_data::Instrument::Violin,
    //     music_data::Instrument::Finger_Bass,
    //     music_data::Instrument::Shamisen
    // };

    constexpr music_data::Instrument DefaultInstrument{music_data::Instrument::Grand_Piano};
    constexpr units::machine::Tempo DefaultTempo{100};
    constexpr units::machine::Volume DefaultVolume{10};

    constexpr units::machine::Articulation DefaultArticulation{units::machine::Articulation::Normal};

} // namespace constants::midi
