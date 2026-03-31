#include "toolbar.hpp"

using namespace interface;

// Toolbar::Toolbar(SystemState &systemState)
//     : systemState_{systemState}
// {}


void Toolbar::draw(SystemState &systemState){
    drawSystemAction(systemState);
    drawStateControl(systemState);
    drawToolPalette(systemState);
    drawOption(systemState);
}