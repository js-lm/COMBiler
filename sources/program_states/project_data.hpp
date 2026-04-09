#pragma once

#include <array>
#include <vector>
#include <optional>
#include <string>

#include "aliases.hpp"
#include "constants.hpp"

namespace program_states{

    struct ProjectData{

        struct Metadata{

            int numberOfPages{16};

            int tempo{16};
            int notePerPage{32};

            // std::string author;
            // std::string 

        } metadata{};

        // struct InstrumentChannel{
        using InstrumentChannel = std::array<
            std::optional<music_data::InstrumentChannelData>, 
            constants::project_data::MaximumNotePerPage
        >;

        // };

        using CommandChannel = std::array<
            std::optional<command::CommandToken>, 
            constants::project_data::MaximumNotePerPage
        >;


        struct Page{
            std::array<InstrumentChannel, constants::project_data::NumberOfInstrumentChannels> instrumentChannels;

            CommandChannel commandChannel;

            std::optional<int> noteInThisPage{};
        };

        std::vector<Page> pages;


        
        struct Transient{
            int currentPageNumber{1};
            int selectedChannelListViewIndex{0};

        } transient{};

    };


    namespace DEBUG_preset{

        inline ProjectData projectData{
            .metadata{
                .notePerPage{16}
            },

            .pages{std::vector<ProjectData::Page>{

                ProjectData::Page{
                    .instrumentChannels{
                        ProjectData::InstrumentChannel{
                            music_data::Note::F3,
                            music_data::Note::F3,
                            music_data::Note::C4,
                            music_data::Note::C4,
                            music_data::Note::D4,
                            music_data::Note::D4,
                            music_data::Note::C4,
                            music_data::Instrument::Grand_Piano,
                            music_data::Note::A_Sharp_3,
                            music_data::Note::A_Sharp_3,
                            music_data::Note::A3,
                            music_data::Note::A3,
                            music_data::Note::G3,
                            music_data::Note::G3,
                            music_data::Note::F3,
                            std::nullopt
                        },
                        ProjectData::InstrumentChannel{
                            music_data::Instrument::Steel_Acoustic_Guitar,
                            music_data::Instrument::Nylon_Acoustic_Guitar,
                            music_data::Instrument::Sitar,
                            music_data::Note::C3,
                            music_data::Instrument::Synthesizer_Bass,
                            music_data::Note::D3,
                            music_data::Instrument::Pad_2_Warm,
                            music_data::Note::C3,
                            music_data::Instrument::Banjo,
                            music_data::Note::A_Sharp_2,
                            music_data::Instrument::Glockenspiel,
                            music_data::Note::A2,
                            music_data::Instrument::Oboe,
                            music_data::Note::G2,
                            music_data::Instrument::Cello,
                            music_data::Note::F2,
                        },
                        ProjectData::InstrumentChannel{
                            music_data::Note::F_Sharp_2,
                            music_data::Note::G2,
                            music_data::Note::G_Sharp_2,
                            music_data::Note::A2,
                            music_data::Note::A_Sharp_2,
                            music_data::Note::B2,
                            music_data::Note::C3,
                            std::nullopt,
                            music_data::Note::E3,
                            music_data::Note::F3,
                            music_data::Note::F_Sharp_3,
                            music_data::Note::G3,
                            music_data::Note::G3,
                            music_data::Note::G3,
                            music_data::Note::F3,
                            std::nullopt
                        },
                        ProjectData::InstrumentChannel{
                            music_data::Note::F5,
                            music_data::Note::F5,
                            music_data::Note::C6,
                            music_data::Note::C6,
                            music_data::Note::D6,
                            music_data::Note::D6,
                            music_data::Note::C6,
                            std::nullopt,
                            music_data::Note::A_Sharp_5,
                            music_data::Note::A_Sharp_5,
                            music_data::Note::A5,
                            music_data::Note::A5,
                            music_data::Note::G5,
                            music_data::Note::G5,
                            music_data::Note::F5,
                            std::nullopt
                        }
                    },
                
                    .commandChannel{
                        command::Command{command::Tempo{80}},
                        std::nullopt,
                        command::Command{command::Volume{8, command::Target::Channel_2}},
                        std::nullopt,
                        command::Command{command::Articulation{command::Articulation::Type::Staccato, command::Target::Channel_4}},
                        std::nullopt,
                        command::Command{command::Tempo{26}},
                        std::nullopt,
                        command::Command{command::Volume{1, command::Target::All_Channels}},
                        std::nullopt,
                        command::Command{command::Articulation{command::Articulation::Type::Legato, command::Target::Channel_3}},
                        std::nullopt,
                        command::Command{command::Volume{15, command::Target::Channel_1}},
                        std::nullopt,
                        command::Command{command::Tempo{60}},
                        std::nullopt,
                        command::Command{command::Articulation{command::Articulation::Type::Normal, command::Target::All_Channels}},
                        std::nullopt,
                        command::Command{command::Volume{4, command::Target::Channel_4}},
                        std::nullopt,
                        command::Command{command::Tempo{100}},
                        std::nullopt,
                        command::Command{command::Articulation{command::Articulation::Type::Sustain, command::Target::Channel_1}},
                        std::nullopt,
                        command::Command{command::Volume{5, command::Target::Channel_3}},
                        std::nullopt,
                        command::Command{command::Articulation{command::Articulation::Type::Infinite, command::Target::Channel_2}},
                        std::nullopt,
                        command::Command{command::Tempo{2}},
                        std::nullopt,
                        command::Command{command::Volume{7, command::Target::All_Channels}},
                        std::nullopt
                    },

                    .noteInThisPage{32}
                
                
                
                },

                ProjectData::Page{
                    .instrumentChannels{
                            ProjectData::InstrumentChannel{
                            music_data::Note::C4,
                            music_data::Note::C4,
                            music_data::Note::A_Sharp_3,
                            music_data::Note::A_Sharp_3,
                            music_data::Note::A3,
                            music_data::Note::A3,
                            music_data::Note::G3,
                            std::nullopt,
                            music_data::Note::C4,
                            music_data::Note::C4,
                            music_data::Note::A_Sharp_3,
                            music_data::Note::A_Sharp_3,
                            music_data::Note::A3,
                            music_data::Note::A3,
                            music_data::Note::G3,
                            std::nullopt
                        }
                    }
                },

                ProjectData::Page{
                    .instrumentChannels{
                        ProjectData::InstrumentChannel{
                            music_data::Note::F3,
                            music_data::Note::F3,
                            music_data::Note::C4,
                            music_data::Note::C4,
                            music_data::Note::D4,
                            music_data::Note::D4,
                            music_data::Note::C4,
                            std::nullopt,
                            music_data::Note::A_Sharp_3,
                            music_data::Note::A_Sharp_3,
                            music_data::Note::A3,
                            music_data::Note::A3,
                            music_data::Note::G3,
                            music_data::Note::G3,
                            music_data::Note::F3,
                            std::nullopt
                        }
                    }
                }

            }}

        };


    } // namespace DEBUG_preset




} // namespace project_data