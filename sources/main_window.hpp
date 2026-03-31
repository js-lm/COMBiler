#pragma once

#include "system_states.hpp"

class MainWindow{
private:
    SystemState systemState_;

    


public:
    MainWindow() = default;
    ~MainWindow() = default;

    void update();
    void draw();

public:
    

};