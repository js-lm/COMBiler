#include "toolbar.hpp"

#include <raygui.h>

using namespace interface;

void Toolbar::draw(program_states::InterfaceContext &context){
    drawSystemAction(context);
    
    if(context.interface.navigationBar.isPageSelectEnabled) GuiDisable();
    drawStateControl(context);
    drawToolPalette(context);
    drawOption(context);
    if(context.interface.navigationBar.isPageSelectEnabled) GuiEnable();
}