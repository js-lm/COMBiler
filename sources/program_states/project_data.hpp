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
            int noteInThisPage{32};
            int barsPerPage{4};

            // std::string author;
            // std::string 

        } metadata{};

        // struct InstrumentChannel{
        using InstrumentChannel = std::array<
            std::optional<music_data::InstrumentChannelData>, 
            constants::project_data::MaximumNotePerPage
        >;

        // };


        struct Page{
            std::array<InstrumentChannel, constants::project_data::NumberOfInstrumentChannels> instrumentChannels;

            // ChordChannel chordChannel;

            std::optional<int> noteInThisPage{};
            std::optional<int> barsPerPage{};
        };

        std::vector<Page> pages;

    };


    namespace DEBUG_preset{

        inline ProjectData projectData{
            .metadata{
                .noteInThisPage{16}
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
                            music_data::Note::F2,
                            music_data::Instrument::Sitar,
                            music_data::Note::C3,
                            music_data::Instrument::Piccolo,
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
                            std::nullopt
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
                    }
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