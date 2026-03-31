#include "navigation_bar.hpp"

#include <raygui.h>

#include "layout.hpp"
#include "labels.hpp"

using namespace interface;
using namespace constants::layout::navigation_bar;
using namespace constants::labels::navigation_bar;

void NavigationBar::drawTimeline(SystemState &systemState){
	GuiGroupBox(TimelineGroupBox, TimelineGroupBoxText);
	GuiScrollPanel(
		Rectangle{
			TimelineScrollPanel.x,
			TimelineScrollPanel.y,
			TimelineScrollPanel.width - systemState.navigationBar.timelineScrollPanelBoundsOffset.x,
			TimelineScrollPanel.height - systemState.navigationBar.timelineScrollPanelBoundsOffset.y
		},
		nullptr,
		TimelineScrollPanel,
		&systemState.navigationBar.timelineScrollPanelScrollOffset,
		&systemState.navigationBar.timelineScrollPanelScrollView
	);
	GuiLabel(TimelineLabel, TimelineLabelText);
	GuiLabel(PageNumberLabel, PageNumberLabelText);
	systemState.navigationBar.isPageCopyButtonPressed = GuiButton(PageCopyButton, PageCopyButtonText);
	systemState.navigationBar.isPagePasteButtonPressed = GuiButton(PagePasteButton, PagePasteButtonText);
	systemState.navigationBar.isPageCutButtonPressed = GuiButton(PageCutButton, PageCutButtonText);
	GuiToggle(PageSelectToggle, PageSelectToggleText, &systemState.navigationBar.isPageSelectEnabled);
}
