#pragma once

#include "Window.h"
#include "Config.h"

class Engine
{
public:
    bool Initialize();
    void Run();
    void Shutdown();

private:
    void ProcessInput();
    void Render();

    Window _window;
    Config _config;

    bool _isRunning = false;
};