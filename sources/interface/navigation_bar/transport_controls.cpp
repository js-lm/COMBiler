#include "navigation_bar.hpp"

#include <raygui.h>

#include "layout.hpp"
#include "labels.hpp"

using namespace interface;
using namespace constants::layout::navigation_bar;
using namespace constants::labels::navigation_bar;

void NavigationBar::drawTransportControls(SystemState &systemState){
	GuiGroupBox(TransportControlsGroupBox, TransportControlsGroupBoxText);
	systemState.navigationBar.isPreviousPageButtonPressed = GuiButton(PreviousPageButton, PreviousPageButtonText);
	systemState.navigationBar.isNextPageButtonPressed = GuiButton(NextPageButton, NextPageButtonText);
	systemState.navigationBar.isPlayButtonPressed = GuiButton(PlayButton, PlayButtonText);
	systemState.navigationBar.isStopButtonPressed = GuiButton(StopButton, StopButtonText);
	GuiToggle(PageRepeatToggle, PageRepeatToggleText, &systemState.navigationBar.isPageRepeatEnabled);
}
