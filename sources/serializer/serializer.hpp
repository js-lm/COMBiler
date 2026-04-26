#pragma once

#include <string>

#include "program_states/project_data.hpp"

#include "command/command_type.hpp"

class Serializer{
public:
    Serializer() = default;
    ~Serializer() = default;

    std::string toYaml(const program_states::ProjectData &data) const;
    program_states::ProjectData toProjectData(const std::string &data) const;

private:
    std::string emitChannelData(const std::optional<music_data::InstrumentChannelData> &channelData) const;
    std::string emitCommandData(const std::optional<command::CommandToken> &commandToken) const;

    std::optional<music_data::InstrumentChannelData> parseChannelData(const std::string &data) const;
    std::optional<command::CommandToken> parseCommandData(const std::string &data) const;

    std::string trimToken(const std::string &data) const;
};