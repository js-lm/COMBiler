#include "navigation_bar.hpp"

using namespace interface;

void NavigationBar::draw(program_states::Context &context){
	drawTransportControls(context);
	drawTimeline(context);
	drawPerPageSettings(context);
	drawStatus(context);
}
