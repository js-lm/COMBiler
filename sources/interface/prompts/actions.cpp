#include "prompts.hpp"

using namespace interface;

void Prompts::commit(program_states::InterfaceContext &context){
	auto &state{context.interface.prompts};

	const auto selectedConstantType{constants::prompts::constants_manager::typeFromIndex(state.constantsManagerTypeToggleGroupIndex)};
	if(selectedConstantType == constants::prompts::constants_manager::Type::None){
		state.draft.commandType = std::nullopt;
	}else{
		state.draft.commandType = selectedConstantType;
	}

    state.hasModified = false;

    state.hasRequestedCommit = true;
}

void Prompts::load(program_states::InterfaceContext &context){
    auto &state{context.interface.prompts};

    state.loadedConstantIndex = state.draft.constantIndex;
    state.selectedCommandTool = constants::prompts::CommandPrompt::Constant;
    state.activeCommandPrompt = constants::prompts::CommandPrompt::Constant;
    state.isConstantsManagerWindowVisible = false;
    state.isCommandWindowVisible = false;
    
    context.interface.sidebar.selectedChannelListViewIndex = constants::sidebar::SystemChannelListViewIndex;
}