#include "command_palette.hpp"

#include <utility>

#include "constants.hpp"

using namespace command;

void CommandPalette::set(
    ConstantIndex       index,
    const std::string   &string,
    const Command       &commandData
){
    if(index >= constants::project_data::MaximumNumberOfConstants){
        return;
    }

    if(!names_[index].empty()) indicesMap_.erase(names_[index]);

    commands_[index] = commandData;
    names_[index] = string;
    indicesMap_[string] = index;
}

std::optional<Command> CommandPalette::get(ConstantIndex index) const{
    if(index >= constants::project_data::MaximumNumberOfConstants){
        return std::nullopt;
    }

    if(names_[index].empty()) return std::nullopt;
    
    return commands_[index];
}

std::optional<Command> CommandPalette::get(const std::string &string) const{
    auto iterator{indicesMap_.find(string)};

    if(iterator == indicesMap_.end()) return std::nullopt;

    return commands_[iterator->second];
}

std::optional<Command> CommandPalette::get(const CommandToken &token) const{
    if(const auto *command{std::get_if<Command>(&token)}){
        return *command;
    }else if(const auto *constantIndex{std::get_if<ConstantIndex>(&token)}){
        return get(*constantIndex);
    }
    return std::nullopt;
}

std::optional<ConstantIndex> CommandPalette::getIndex(const std::string &string) const{
    auto iterator{indicesMap_.find(string)};

    if(iterator == indicesMap_.end()) return std::nullopt;

    return iterator->second;
}

CommandPalette::List CommandPalette::getList() const{
    List list{};

    for(ConstantIndex index{0}; index < constants::project_data::MaximumNumberOfConstants; index++){
        list[index] = {names_[index], commands_[index]};
    }

    return list;
}

void CommandPalette::remove(ConstantIndex index){
    if(index >= constants::project_data::MaximumNumberOfConstants){
        return;
    }

    if(!names_[index].empty()){
        indicesMap_.erase(names_[index]);
        names_[index].clear();
        commands_[index] = std::nullopt;
    }
}

void CommandPalette::remove(const std::string &string){
    auto iterator{indicesMap_.find(string)};

    if(iterator != indicesMap_.end()){
        ConstantIndex index{iterator->second};
        names_[index].clear();
        commands_[index] = std::nullopt;
        indicesMap_.erase(iterator);
    }
}

void CommandPalette::swap(ConstantIndex firstIndex, ConstantIndex secondIndex){
    if(firstIndex >= constants::project_data::MaximumNumberOfConstants || secondIndex >= constants::project_data::MaximumNumberOfConstants){
        return;
    }

    if(firstIndex == secondIndex) return;

    std::swap(commands_[firstIndex], commands_[secondIndex]);
    std::swap(names_[firstIndex], names_[secondIndex]);

    if(!names_[firstIndex].empty()){
        indicesMap_[names_[firstIndex]] = firstIndex;
    }

    if(!names_[secondIndex].empty()){
        indicesMap_[names_[secondIndex]] = secondIndex;
    }
}

void CommandPalette::swap(const std::string &firstString, const std::string &secondString){
    auto firstIterator{indicesMap_.find(firstString)};
    auto secondIterator{indicesMap_.find(secondString)};

    if(firstIterator == indicesMap_.end() || secondIterator == indicesMap_.end()){
        return;
    }

    swap(firstIterator->second, secondIterator->second);
}