#include "toolbar.hpp"

#include <raygui.h>

#include "utilities/project_utilities.hpp"

using namespace interface;

void Toolbar::draw(program_states::InterfaceContext &context){
    drawSystemAction(context);
    
    if(context.interface.navigationBar.isPageSelectEnabled) GuiDisable();
    drawStateControl(context);
    drawToolPalette(context);
    if(context.interface.navigationBar.isPageSelectEnabled) GuiEnable();

    drawOption(context);
}