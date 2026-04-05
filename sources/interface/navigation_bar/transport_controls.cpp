#include "navigation_bar.hpp"

#include <raygui.h>

#include "labels.hpp"

#include "interface/utilities.hpp"

using namespace interface;
using namespace constants::labels::navigation_bar;

void NavigationBar::drawTransportControls(program_states::InterfaceContext &context){
	const auto anchor{context.layout.anchor.navigationBar.transportControls};
	const auto &bounds{context.layout.bounds.navigationBar.transportControls};

	GuiGroupBox(calculateBoundsAtAnchor(anchor, bounds.groupBox), TransportControlsGroupBoxText);
	context.interface.navigationBar.isPreviousPageButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.previousPageButton), PreviousPageButtonText);
	context.interface.navigationBar.isNextPageButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.nextPageButton), NextPageButtonText);
	context.interface.navigationBar.isPlayButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.playButton), PlayButtonText);
	context.interface.navigationBar.isStopButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.stopButton), StopButtonText);
	GuiToggle(calculateBoundsAtAnchor(anchor, bounds.pageRepeatToggle), PageRepeatToggleText, &context.interface.navigationBar.isPageRepeatEnabled);
}
