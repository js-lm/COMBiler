#pragma once

#include <array>
#include <vector>
#include <optional>

#include "aliases.hpp"
#include "constants.hpp"

#include "command/command_type.hpp"
#include "command/command_palette.hpp"

namespace program_states{

    struct ProjectData{
        struct Metadata{
            int numberOfPages{1};
            int notePerPage{constants::project_data::MaximumNotePerPage};

            char title[constants::project_data::MetadataTitleMaximumLength + 1]{""};
            char author[constants::project_data::MetadataAuthorMaximumLength + 1]{""};
            char creationDate[constants::project_data::MetadataDateMaximumLength + 1]{""};
            char modificationDate[constants::project_data::MetadataDateMaximumLength + 1]{""};

            float maximumTempo{constants::project_data::DefaultMaximumTempo};
            bool isReadOnly{false};
        } metadata{};

        command::CommandPalette commandPalette;

        using InstrumentChannel = std::array<
            std::optional<music_data::InstrumentChannelData>, 
            constants::project_data::MaximumNotePerPage
        >;

        using CommandChannel = std::array<
            std::optional<command::CommandToken>, 
            constants::project_data::MaximumNotePerPage
        >;

        struct Page{
            std::array<InstrumentChannel, constants::project_data::NumberOfInstrumentChannels> instrumentChannels;

            CommandChannel commandChannel;

            std::optional<int> noteInThisPage{};
            size_t modificationCounter{0};
        };

        std::vector<Page> pages{1};
        
        struct Transient{
            int currentPageNumber{1};
            int selectedChannelListViewIndex{0};

        } transient{};
    };

} // namespace project_data