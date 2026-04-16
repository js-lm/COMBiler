#include "navigation_bar.hpp"

using namespace interface;

const TimelineManager *NavigationBar::timelineManager_{nullptr};

void NavigationBar::bindTimelineManager(const TimelineManager *timelineManager){
    timelineManager_ = timelineManager;
}

void NavigationBar::draw(program_states::InterfaceContext &context){
	drawTransportControls(context);
	drawTimeline(context);
	drawPerPageSettings(context);
	drawStatus(context);
}
