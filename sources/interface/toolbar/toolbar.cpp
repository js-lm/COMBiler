#include "toolbar.hpp"

using namespace interface;

void Toolbar::draw(program_states::InterfaceContext &context){
    drawSystemAction(context);
    drawStateControl(context);
    drawToolPalette(context);
    drawOption(context);
}