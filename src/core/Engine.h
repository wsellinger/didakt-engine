#pragma once

#include "Config.h"
#include "Window.h"

#include "../providers/sdl/SDLInputProvider.h"

#include "../ecs/RegistryManager.h"
#include "../ecs/systems/RenderSystem.h"
#include "../ecs/systems/MovementSystem.h"

#include "../input/InputManager.h"

#include "../render/AssetManager.h"
#include "../render/Camera.h"

#include <SDL_stdinc.h>

//Engine

class Engine
{
public:
    Engine() = default;
    ~Engine() = default;

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
    Engine(Engine&&) = delete;
    Engine& operator=(Engine&&) = delete;

    bool Initialize();
    void Run();
    void Shutdown();

private:
    void ProcessEvents();
    void FixedUpdate(double deltaTime);
    void FrameUpdate(double deltaTime);
    void Render();

    //Providers
    SDLInputProvider _inputProvider;

    Config _config;
    Window _window;

    AssetManager _assetManager{};
    RegistryManager _registryManager{};
    InputManager _inputManager{ _inputProvider };

    RenderSystem _renderSystem;
    MovementSystem _movementSystem;

    Camera _camera;

    bool _isRunning = false;
};

//Utility Classes

class TimeTracker
{
public:
    TimeTracker(Uint64 initialTime, Uint64 frequency) :
        _time(initialTime), _frequency(frequency) {}

    double GetDeltaTime(Uint64 currentTime);

private:
    static constexpr double MAX_DELTA_TIME = 0.05;

    Uint64 _time = 0;
    Uint64 _frequency = 0;
};

class FixedTimer
{
public:

    void Increment(double deltaTime) { _accumulator += deltaTime; }
    bool Update();

    double GetTimestep() const { return FIXED_TIMESTEP; }

private:
    static constexpr double FIXED_TIMESTEP = 1.0 / 60.0;

    double _accumulator = 0.0;
};

class FPSMeter
{
public:
    FPSMeter(Uint64 initialTime, Uint64 frequency) :
        _time(initialTime), _frequency(frequency) {}

    bool Update(Uint64 currentTime);
    int GetFPS() const { return _fps; }

private:
    static const int UPDATE_TIME = 1;

    int _fps = 0;

    Uint64 _time = 0;
    Uint64 _frequency = 0;
    int _frames = 0;
};