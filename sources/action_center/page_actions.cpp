#include "action_center.hpp"

#include <algorithm>

#include "debug_utilities.hpp"

void ActionCenter::addPage(int requestedPageInsertionIndex){
    
	if(!stagedSlot_->data) return;

	beginAction();

	auto &projectData{*stagedSlot_->data};
	int insertionIndex{requestedPageInsertionIndex};

	if(insertionIndex == constants::action_center::InvalidPageInsertionIndex){

		insertionIndex = static_cast<int>(projectData.pages.size());

	}

	insertionIndex = std::clamp(
		insertionIndex,
		constants::action_center::MinimumPageIndex,
		static_cast<int>(projectData.pages.size())
	);


	projectData.pages.insert(
		projectData.pages.begin() + insertionIndex,
		program_states::ProjectData::Page{}
	);
	projectData.metadata.numberOfPages = static_cast<int>(projectData.pages.size());
	projectData.transient.currentPageNumber = insertionIndex + constants::action_center::FirstPageNumber;
	const int numberOfPages{projectData.metadata.numberOfPages};

	finishAction();

    
	DEBUG_PRINT(
		"addPage(); insertionIndex: {}, numberOfPages: {}",
		insertionIndex,
		numberOfPages
	);



}

void ActionCenter::movePage(int sourcePageIndex, int targetInsertionIndex){
	if(!stagedSlot_->data) return;
	const int pageCount{static_cast<int>(stagedSlot_->data->pages.size())};
	if(pageCount <= constants::action_center::FirstPageNumber) return;

	beginAction();

	auto &projectData{*stagedSlot_->data};

	const int clampedSourcePageIndex{std::clamp(
		sourcePageIndex,
		constants::action_center::MinimumPageIndex,
		pageCount - constants::action_center::FirstPageNumber
	)};
	const int clampedTargetInsertionIndex{std::clamp(
		targetInsertionIndex,
		constants::action_center::MinimumPageIndex,
		pageCount
	)};


	auto movedPage{projectData.pages[clampedSourcePageIndex]};
	projectData.pages.erase(projectData.pages.begin() + clampedSourcePageIndex);

	int insertionIndex{clampedTargetInsertionIndex};
	if(clampedSourcePageIndex < insertionIndex){
		insertionIndex -= constants::action_center::FirstPageNumber;
	}
	insertionIndex = std::clamp(
		insertionIndex,
		constants::action_center::MinimumPageIndex,
		static_cast<int>(projectData.pages.size())
	);

	projectData.pages.insert(projectData.pages.begin() + insertionIndex, std::move(movedPage));
	projectData.metadata.numberOfPages = static_cast<int>(projectData.pages.size());
	projectData.transient.currentPageNumber = insertionIndex + constants::action_center::FirstPageNumber;

	finishAction();


	DEBUG_PRINT(
		"movePage()- sourcePageIndex: {}, targetInsertionIndex: {}, insertionIndex: {}",
		clampedSourcePageIndex,
		clampedTargetInsertionIndex,
		insertionIndex
	);
}

void ActionCenter::removePage(){


    
}

void ActionCenter::copyPage(){


    
}

void ActionCenter::pastePage(){


    
}