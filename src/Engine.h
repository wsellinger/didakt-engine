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

class FPSMeter
{
public:
    FPSMeter(Uint64 initialTime, Uint64 frequency) : 
        _timer(initialTime), _frequency(frequency) {}

    bool Update(Uint64 currentTime);
    int GetFPS() const { return _fps; }

private:
    int _fps = 0;

    Uint64 _timer = 0;
    Uint64 _frequency = 0;
    int _frames = 0;

    static const int UPDATE_TIME = 1;
};