#include "navigation_bar.hpp"

#include <raygui.h>

#include "labels.hpp"

#include "interface/utilities.hpp"

using namespace interface;
namespace bar_labels = constants::labels::navigation_bar;

void NavigationBar::drawStatus(program_states::InterfaceContext &context){
	const auto anchor{context.layout.anchor.navigationBar.status};
	const auto &bounds{context.layout.bounds.navigationBar.status};

	const auto groupBox{calculateBoundsAtAnchor(anchor, bounds.groupBox)};

	GuiGroupBox(groupBox, bar_labels::StatusGroupBoxText);
}
