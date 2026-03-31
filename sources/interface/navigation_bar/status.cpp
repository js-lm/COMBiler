#include "navigation_bar.hpp"

#include <raygui.h>

#include "layout.hpp"
#include "labels.hpp"

using namespace interface;
using namespace constants::layout::navigation_bar;
using namespace constants::labels::navigation_bar;

void NavigationBar::drawStatus(SystemState &systemState){
	GuiGroupBox(StatusGroupBox, StatusGroupBoxText);
}
