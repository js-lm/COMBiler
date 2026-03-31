#include "navigation_bar.hpp"

#include <raygui.h>

#include "labels.hpp"

#include "interface/utilities.hpp"

using namespace interface;
using namespace constants::labels::navigation_bar;

void NavigationBar::drawTimeline(program_states::Context &context){
	const auto anchor{context.layout.anchor.navigationBar.timeline};
	const auto &bounds{context.layout.bounds.navigationBar.timeline};

	const auto timelineScrollPanel{calculateBoundsAtAnchor(anchor, bounds.scrollPanel)};

	GuiGroupBox(calculateBoundsAtAnchor(anchor, bounds.groupBox), TimelineGroupBoxText);
	GuiScrollPanel(
		Rectangle{
			timelineScrollPanel.x,
			timelineScrollPanel.y,
			timelineScrollPanel.width - context.interface.navigationBar.timelineScrollPanelBoundsOffset.x,
			timelineScrollPanel.height - context.interface.navigationBar.timelineScrollPanelBoundsOffset.y
		},
		nullptr,
		timelineScrollPanel,
		&context.interface.navigationBar.timelineScrollPanelScrollOffset,
		&context.interface.navigationBar.timelineScrollPanelScrollView
	);
	GuiLabel(calculateBoundsAtAnchor(anchor, bounds.timelineLabel), TimelineLabelText);
	GuiLabel(calculateBoundsAtAnchor(anchor, bounds.pageNumberLabel), PageNumberLabelText);
	context.interface.navigationBar.isPageCopyButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.pageCopyButton), PageCopyButtonText);
	context.interface.navigationBar.isPagePasteButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.pagePasteButton), PagePasteButtonText);
	context.interface.navigationBar.isPageCutButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.pageCutButton), PageCutButtonText);
	GuiToggle(calculateBoundsAtAnchor(anchor, bounds.pageSelectToggle), PageSelectToggleText, &context.interface.navigationBar.isPageSelectEnabled);
}
