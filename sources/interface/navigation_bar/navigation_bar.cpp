#include "navigation_bar.hpp"

using namespace interface;

void NavigationBar::draw(SystemState &systemState){
	drawTransportControls(systemState);
	drawTimeline(systemState);
	drawPerPageSettings(systemState);
	drawStatus(systemState);
}
