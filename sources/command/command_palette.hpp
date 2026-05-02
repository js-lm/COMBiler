#pragma once

#include "command_type.hpp"
#include "constants.hpp"

namespace command{

    class CommandPalette{
    public:
        using List = std::array<std::pair<std::string, std::optional<Command>>, constants::project_data::MaximumNumberOfConstants>;

    private:
        std::array<std::optional<Command>, constants::project_data::MaximumNumberOfConstants> commands_{};

        std::array<std::string, constants::project_data::MaximumNumberOfConstants> names_{};
        std::unordered_map<std::string, ConstantIndex> indicesMap_{};

    public:
        CommandPalette() = default;
        ~CommandPalette() = default;

        void set(command::ConstantIndex index, const std::string &string, const command::Command &command);

        std::optional<command::Command> get(command::ConstantIndex index) const;
        std::optional<command::Command> get(const std::string &string) const;
        std::optional<command::Command> get(const command::CommandToken &token) const;

        std::optional<command::ConstantIndex> getIndex(const std::string &string) const;

        List getList() const;

        void remove(command::ConstantIndex index);
        void remove(const std::string &string);

        void swap(command::ConstantIndex firstIndex, command::ConstantIndex secondIndex);
        void swap(const std::string &firstString, const std::string &secondString);
    };

} // namespace command