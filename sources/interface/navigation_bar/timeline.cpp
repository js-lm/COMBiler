#include "navigation_bar.hpp"

#include <raygui.h>

#include "labels.hpp"

#include "interface/utilities.hpp"

#include <algorithm>

using namespace interface;
using namespace constants::labels::navigation_bar;

void NavigationBar::drawTimeline(program_states::InterfaceContext &context){
	const auto anchor{context.layout.anchor.navigationBar.timeline};
	const auto &bounds{context.layout.bounds.navigationBar.timeline};

	const auto timelineScrollPanel{calculateBoundsAtAnchor(anchor, bounds.scrollPanel)};

	GuiGroupBox(calculateBoundsAtAnchor(anchor, bounds.groupBox), TimelineGroupBoxText);
	GuiStatusBar(calculateBoundsAtAnchor(anchor, bounds.statusBar), TimelineStatusBarText);
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

	int maximumPageNumber{1};
	int selectedPageNumber{1};
	if(const auto projectDataSlot{context.system.project.data.lock()}; projectDataSlot && projectDataSlot->data){
		maximumPageNumber = projectDataSlot->data->pages.size();
		selectedPageNumber = context.system.project.currentPage;
	}
	GuiLabel(
		calculateBoundsAtAnchor(anchor, bounds.pageNumberValueLabel),
		TextFormat("%d/%d", selectedPageNumber, maximumPageNumber)
	);

	context.interface.navigationBar.isPageCopyButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.pageCopyButton), PageCopyButtonText);
	context.interface.navigationBar.isPagePasteButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.pagePasteButton), PagePasteButtonText);
	context.interface.navigationBar.isPageCutButtonPressed = GuiButton(calculateBoundsAtAnchor(anchor, bounds.pageCutButton), PageCutButtonText);
	GuiToggle(calculateBoundsAtAnchor(anchor, bounds.pageSelectToggle), PageSelectToggleText, &context.interface.navigationBar.isPageSelectEnabled);
}
