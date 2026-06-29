#include "action_center.hpp"

#include <algorithm>

#include "debug_utilities.hpp"

#include "program_states/context.hpp"

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

void ActionCenter::removePage(int startIndex, int endIndex){
    if(!stagedSlot_->data) return;
    const int pageCount{static_cast<int>(stagedSlot_->data->pages.size())};

    beginAction();

    auto &projectData{*stagedSlot_->data};
    const int minIndex{std::max(0, std::min(startIndex, endIndex))};
    const int maxIndex{std::min(pageCount - 1, std::max(startIndex, endIndex))};
    if(minIndex > maxIndex){
        finishAction();
        return;
    }

    projectData.pages.erase(projectData.pages.begin() + minIndex, projectData.pages.begin() + maxIndex + 1);

    if(projectData.pages.empty()){
        projectData.pages.emplace_back();
    }

    projectData.metadata.numberOfPages = static_cast<int>(projectData.pages.size());

    if(projectData.transient.currentPageNumber - constants::action_center::FirstPageNumber > maxIndex){
        projectData.transient.currentPageNumber -= (maxIndex - minIndex + 1);
    }else if(projectData.transient.currentPageNumber - constants::action_center::FirstPageNumber >= minIndex){
        projectData.transient.currentPageNumber = minIndex + constants::action_center::FirstPageNumber;
    }
    projectData.transient.currentPageNumber = std::clamp(
        projectData.transient.currentPageNumber,
        constants::action_center::FirstPageNumber,
        projectData.metadata.numberOfPages
    );

    finishAction();
}

void ActionCenter::copyPage(program_states::Interface &interface, int startIndex, int endIndex){
    if(!stagedSlot_->data) return;
    const int pageCount{static_cast<int>(stagedSlot_->data->pages.size())};

    auto &projectData{*stagedSlot_->data};
    const int minIndex{std::max(0, std::min(startIndex, endIndex))};
    const int maxIndex{std::min(pageCount - 1, std::max(startIndex, endIndex))};
    
    if(minIndex > maxIndex) return;

    auto &clipboard{interface.timelineClipboard};
    clipboard.copiedPages.clear();
    clipboard.hasCopiedPages = true;
    
    for(int index{minIndex}; index <= maxIndex; index++){
        clipboard.copiedPages.push_back(projectData.pages[index]);
    }
}

void ActionCenter::pastePage(program_states::Interface &interface, int insertionIndex){
    if(!stagedSlot_->data) return;
    auto &clipboard{interface.timelineClipboard};
    if(!clipboard.hasCopiedPages || clipboard.copiedPages.empty()) return;

    beginAction();
    auto &projectData{*stagedSlot_->data};

    const int index{std::clamp(insertionIndex, 0, static_cast<int>(projectData.pages.size()))};

    projectData.pages.insert(
        projectData.pages.begin() + index,
        clipboard.copiedPages.begin(),
        clipboard.copiedPages.end()
    );

    projectData.metadata.numberOfPages = static_cast<int>(projectData.pages.size());
    projectData.transient.currentPageNumber = index + static_cast<int>(clipboard.copiedPages.size()) + constants::action_center::FirstPageNumber - 1;

    finishAction();
}

void ActionCenter::cutPage(program_states::Interface &interface, int startIndex, int endIndex){
    copyPage(interface, startIndex, endIndex);
    removePage(startIndex, endIndex);
}