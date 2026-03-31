#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#include "main_window.hpp"

int main(){
    MainWindow application{};
    return application.run();
}