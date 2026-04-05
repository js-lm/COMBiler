#include "sidebar.hpp"

using namespace interface;

void Sidebar::draw(program_states::InterfaceContext &context){
    drawInspector(context);
}