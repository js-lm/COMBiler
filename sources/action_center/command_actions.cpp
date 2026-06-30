#include "action_center.hpp"

#include <algorithm>

#include "debug_utilities.hpp"

#include <magic_enum/magic_enum.hpp>

#include "utilities/project_utilities.hpp"

void ActionCenter::addCommand(int pageNumber, int noteIndex, const program_states::Interface::Prompts &promptState){
	auto &page{utilities::pageByNumber(*stagedSlot_->data, pageNumber)};

	beginAction();

	// stagedSlot_->data->transient.currentPageNumber = pageNumber;
	// stagedSlot_->data->transient.selectedChannelListViewIndex = constants::sidebar::SystemChannelListViewIndex;

	if(promptState.selectedCommandTool == constants::prompts::CommandPrompt::Constant){
		page.commandChannel[noteIndex] = promptState.loadedConstantIndex;
	}else{
		page.commandChannel[noteIndex] = commandFromPromptState(promptState);
	}

	DEBUG_PRINT(
		"addCommand(); pageNumber: {}, noteIndex: {}, selectedCommandTool: {}",
		pageNumber,
		noteIndex,
		magic_enum::enum_name(promptState.selectedCommandTool)
	);
}

void ActionCenter::removeCommand(int pageNumber, int noteIndex){
	auto &page{utilities::pageByNumber(*stagedSlot_->data, pageNumber)};

	beginAction();

	// stagedSlot_->data->transient.currentPageNumber = pageNumber;
	// stagedSlot_->data->transient.selectedChannelListViewIndex = constants::sidebar::SystemChannelListViewIndex;

	page.commandChannel[noteIndex] = std::nullopt;

	DEBUG_PRINT(
		"removeCommand(); pageNumber: {}, noteIndex: {}",
		pageNumber,
		noteIndex
	);
}

void ActionCenter::setCommandToken(
	int pageNumber,
	int noteIndex,
	int selectedChannelListViewIndex,
	std::optional<command::CommandToken> commandToken
){
	auto &page{utilities::pageByNumber(*stagedSlot_->data, pageNumber)};

	beginAction();

	// stagedSlot_->data->transient.currentPageNumber = pageNumber;
	// stagedSlot_->data->transient.selectedChannelListViewIndex = selectedChannelListViewIndex;

	page.commandChannel[noteIndex] = std::move(commandToken);
}