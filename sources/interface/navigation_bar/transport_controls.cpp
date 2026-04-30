#include "navigation_bar.hpp"

#include <raygui.h>

#include "labels.hpp"

#include "interface/utilities.hpp"

#include "debug_utilities.hpp"

using namespace interface;
namespace bar_labels = constants::labels::navigation_bar;

void NavigationBar::drawTransportControls(program_states::InterfaceContext &context){
	const auto anchor{context.layout.anchor.navigationBar.transportControls};
	const auto &bounds{context.layout.bounds.navigationBar.transportControls};

	GuiGroupBox(calculateBoundsAtAnchor(anchor, bounds.groupBox), bar_labels::TransportControlsGroupBoxText);
	context.interface.navigationBar.isPreviousPageButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.previousPageButton), bar_labels::PreviousPageButtonText);
	context.interface.navigationBar.isNextPageButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.nextPageButton), bar_labels::NextPageButtonText);


	bool &playingStatus{context.machine.isPlaying};
	bool isPlaying{playingStatus};
	bool isNotPlaying{!playingStatus};
	GuiToggle(calculateBoundsAtAnchor(anchor, bounds.playButton), bar_labels::PlayButtonText, &isPlaying);
	GuiToggle(calculateBoundsAtAnchor(anchor, bounds.stopButton), bar_labels::StopButtonText, &isNotPlaying);
	if(isPlaying != playingStatus || isNotPlaying == playingStatus){
		playingStatus = ! playingStatus;
	}

	// int toggleIndex{0};
	// if(!playingStatus) toggleIndex = 1;
	// GuiToggleGroup(calculateBoundsAtAnchor(anchor, bounds.playButton), ">;O", &toggleIndex);
	// playingStatus = toggleIndex == 0;

	GuiToggle(calculateBoundsAtAnchor(anchor, bounds.pageRepeatToggle), bar_labels::PageRepeatToggleText, &context.interface.navigationBar.isPageRepeatEnabled);
}
