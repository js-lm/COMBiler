#include "prompts.hpp"

using namespace interface;

void Prompts::commit(program_states::InterfaceContext &context){
	auto &state{context.interface.prompts};

    state.hasModified = false;

}

void Prompts::load(program_states::InterfaceContext &context){



}