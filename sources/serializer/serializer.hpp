#pragma once

#include <string>
#include <optional>

#include "program_states/project_data.hpp"

#include "command/command_type.hpp"

class Serializer{
private:
    std::string currentWorkingDirectory_{};
    std::string currentFilename_{};

public:
    Serializer() = default;
    ~Serializer() = default;

    void save(const program_states::ProjectData &data, bool saveAsNewFile = false);
    std::optional<program_states::ProjectData> load();
    std::optional<program_states::ProjectData> load(const std::string &filePath);

    std::string getCurrentFilename() const{ return GetFileNameWithoutExt(currentFilename_.c_str());}

private:
    const std::string toString(const program_states::ProjectData &data) const;
    program_states::ProjectData toProjectData(const std::string &data) const;

private:
    std::string emitChannelData(const std::optional<music_data::InstrumentChannelData> &channelData) const;
    std::string emitCommandToken(const std::optional<command::CommandToken> &commandToken) const;
    std::string emitCommand(const command::Command &command) const;

    std::optional<music_data::InstrumentChannelData> parseChannelData(const std::string &data) const;
    std::optional<command::CommandToken> parseCommandToken(const std::string &data) const;
    command::Command parseCommand(const std::string &data) const;
};