#include "toolbar.hpp"

using namespace interface;

void Toolbar::draw(program_states::Context &context){
    drawSystemAction(context);
    drawStateControl(context);
    drawToolPalette(context);
    drawOption(context);
}