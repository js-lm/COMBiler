#pragma once

#include <cstdint>
#include <variant>
#include <array>
#include <string>
#include <unordered_map>

#include "utilities/enum_map.hpp"

namespace music_data{

    // enum class Channel{
    //     Instrument_1,
    //     Instrument_2,
    //     Instrument_3,
    //     Instrument_4,
    //     System
    // };

    // namespace commands{

    //     using ChangeInstrument = uint8_t;


    // } // commands

    enum class Note : uint8_t{
        C0,
        C_Sharp_0,
        D_Flat_0 = C_Sharp_0,
        D0,
        D_Sharp_0,
        E_Flat_0 = D_Sharp_0,
        E0,
        F0,
        F_Sharp_0,
        G_Flat_0 = F_Sharp_0,
        G0,
        G_Sharp_0,
        A_Flat_0 = G_Sharp_0,
        A0,
        A_Sharp_0,
        B_Flat_0 = A_Sharp_0,
        B0,

        C1,
        C_Sharp_1,
        D_Flat_1 = C_Sharp_1,
        D1,
        D_Sharp_1,
        E_Flat_1 = D_Sharp_1,
        E1,
        F1,
        F_Sharp_1,
        G_Flat_1 = F_Sharp_1,
        G1,
        G_Sharp_1,
        A_Flat_1 = G_Sharp_1,
        A1,
        A_Sharp_1,
        B_Flat_1 = A_Sharp_1,
        B1,

        C2,
        C_Sharp_2,
        D_Flat_2 = C_Sharp_2,
        D2,
        D_Sharp_2,
        E_Flat_2 = D_Sharp_2,
        E2,
        F2,
        F_Sharp_2,
        G_Flat_2 = F_Sharp_2,
        G2,
        G_Sharp_2,
        A_Flat_2 = G_Sharp_2,
        A2,
        A_Sharp_2,
        B_Flat_2 = A_Sharp_2,
        B2,

        C3,
        C_Sharp_3,
        D_Flat_3 = C_Sharp_3,
        D3,
        D_Sharp_3,
        E_Flat_3 = D_Sharp_3,
        E3,
        F3,
        F_Sharp_3,
        G_Flat_3 = F_Sharp_3,
        G3,
        G_Sharp_3,
        A_Flat_3 = G_Sharp_3,
        A3,
        A_Sharp_3,
        B_Flat_3 = A_Sharp_3,
        B3,

        C4,
        C_Sharp_4,
        D_Flat_4 = C_Sharp_4,
        D4,
        D_Sharp_4,
        E_Flat_4 = D_Sharp_4,
        E4,
        F4,
        F_Sharp_4,
        G_Flat_4 = F_Sharp_4,
        G4,
        G_Sharp_4,
        A_Flat_4 = G_Sharp_4,
        A4,
        A_Sharp_4,
        B_Flat_4 = A_Sharp_4,
        B4,

        C5,
        C_Sharp_5,
        D_Flat_5 = C_Sharp_5,
        D5,
        D_Sharp_5,
        E_Flat_5 = D_Sharp_5,
        E5,
        F5,
        F_Sharp_5,
        G_Flat_5 = F_Sharp_5,
        G5,
        G_Sharp_5,
        A_Flat_5 = G_Sharp_5,
        A5,
        A_Sharp_5,
        B_Flat_5 = A_Sharp_5,
        B5,

        C6,
        C_Sharp_6,
        D_Flat_6 = C_Sharp_6,
        D6,
        D_Sharp_6,
        E_Flat_6 = D_Sharp_6,
        E6,
        F6,
        F_Sharp_6,
        G_Flat_6 = F_Sharp_6,
        G6,
        G_Sharp_6,
        A_Flat_6 = G_Sharp_6,
        A6,
        A_Sharp_6,
        B_Flat_6 = A_Sharp_6,
        B6,

        C7,
        C_Sharp_7,
        D_Flat_7 = C_Sharp_7,
        D7,
        D_Sharp_7,
        E_Flat_7 = D_Sharp_7,
        E7,
        F7,
        F_Sharp_7,
        G_Flat_7 = F_Sharp_7,
        G7,
        G_Sharp_7,
        A_Flat_7 = G_Sharp_7,
        A7,
        A_Sharp_7,
        B_Flat_7 = A_Sharp_7,
        B7,

        C8,
        C_Sharp_8,
        D_Flat_8 = C_Sharp_8,
        D8,
        D_Sharp_8,
        E_Flat_8 = D_Sharp_8,
        E8,
        F8,
        F_Sharp_8,
        G_Flat_8 = F_Sharp_8,
        G8,
        G_Sharp_8,
        A_Flat_8 = G_Sharp_8,
        A8,
        A_Sharp_8,
        B_Flat_8 = A_Sharp_8,
        B8
    };

    enum class Instrument : uint8_t{
    // Keyboards
        Grand_Piano,
        Electric_Piano,
        Drawbar_Organ,
        Church_Organ,

    // Percussion
        Music_Box,
        Marimba,
        Tubular_Bells,
        Glockenspiel,
        Vibraphone,
        Kalimba,

    // Woodwinds
        Flute,
        Oboe,
        Clarinet,
        Bassoon,

    // Strings
        Violin,
        Viola,
        Cello,
        Contrabass,
        Pizzicato_Strings,
        Orchestral_Harp,

    // Brass
        Trumpet,
        French_Horn,
        Trombone,
        Tuba,

    // Harmony
        String_Ensemble,
        Choir_Aahs,

    // Guitars
        Nylon_Acoustic_Guitar,
        Steel_Acoustic_Guitar,
        Electric_Guitar,
        Overdriven_Guitar,

    // Basses
        Acoustic_Bass,
        Finger_Bass,
        Fretless_Bass,
        Synthesizer_Bass,

    // Reeds
        Alto_Saxophone,
        Tenor_Saxophone,

    // Synthesizers
        Lead_1_Square,
        Lead_2_Sawtooth,
        Lead_3_Calliope,
        Pad_1_Fantasia,
        Pad_2_Warm,
        Synthesizer_Strings,

    // World
        Shamisen,
        Steel_Drum,
        Sitar,
        Accordion_French,
        Harmonica,
        Banjo,
        Pan_Flute,

    // Drum Sets
        Drum_Sets
    };


    // namespace data_field{

        // struct Note{ int value;};
        // struct InstrumentIndex{ int value;};
        
    using InstrumentChannelData = std::variant<Note, Instrument>;

    // } // namespace midi


    enum class InstrumentFamily{
        Keyboard,
        Percussion,
        Woodwind,
        String,
        Brass,
        Harmony,
        Guitar,
        Bass,
        Reed,
        Synthesizer,
        World,
        Drum_Sets
    };

    // constexpr utilities::EnumMap<InstrumentFamily, Instrument>

} // music_data

namespace command{

    // using Tempo = int;
    // using Volume = uint8_t;

    // enum class Articulation{
    //     Staccato,
    //     Normal,
    //     Legato,
    //     Sustain,
    //     Infinite
    // };

    // using Target = uint8_t;

    // // using Constant = std::variant<Tempo, Volume, Articulation>;
    // using Constants = std::unordered_map<std::string, std::variant<Tempo, Volume, Articulation>>;



    // using Value = std::variant<Tempo, Volume, Articulation, std::string>


    // using Value = std::variant<int, std::string>;
    // // using Value = std::variant<int, char*>;


    // struct Command{
    //     enum class Type{
    //         Tempo,
    //         Volume,
    //         Articulation
    //     } type;

    //     Value value;
    // };

    enum class Target : uint8_t{
        All_Channels,
        Channel_1,
        Channel_2,
        Channel_3,
        Channel_4
    };


    struct Tempo{
        uint8_t tempo{80};
    };

    struct Volume{
        uint8_t volume{8};
        Target target{Target::All_Channels};
    };

    struct Articulation{
        enum class Type : uint8_t{
            Staccato,
            Normal,
            Legato,
            Sustain,
            Infinite
        } articulation;

        Target target{Target::All_Channels};
    };

    using Command = std::variant<Tempo, Volume, Articulation>;

    // // using CommandConstants = std::unordered_map<std::string, Command>;
    // using CommandConstants = std::array<Command, 256>;
    // using CommandConstantStrings = std::array<std::string, 256>;

    using ConstantIndex = uint8_t;

    template<ConstantIndex MaximumSize>
    class CommandConstants{
    private:
        std::array<Command, MaximumSize> commands_{};
        std::unordered_map<std::string, ConstantIndex> indicesMap_;

    public:
        CommandConstants() = default;
        ~CommandConstants() = default;

        void setCommand(ConstantIndex index, const std::string &string, Command &command);

        std::optional<Command> getCommand(ConstantIndex index);
        std::optional<Command> getCommand(const std::string &string);

    };

    using CommandToken = std::variant<Command, ConstantIndex>;

} // namespace commands