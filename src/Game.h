#pragma once
#include "Window.h"

class Game
{
public:
    bool Initialize();
    void Run();
    void Shutdown();

private:
    void ProcessInput();
    void Render();

    Window _window;
    bool _isRunning = false;
};