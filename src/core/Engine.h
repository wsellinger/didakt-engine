#pragma once

#include "Config.h"
#include "Window.h"

#include "../providers/sdl/SDLAssetProvider.h"
#include "../providers/sdl/SDLInputProvider.h"
#include "../providers/sdl/SDLLoggingProvider.h"
#include "../providers/sdl/SDLRenderProvider.h"
#include "../providers/sdl/SDLSystemTimerProvider.h"

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
    virtual ~Engine() = default;

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
    Engine(Engine&&) = delete;
    Engine& operator=(Engine&&) = delete;

    virtual bool Initialize();
    void Run();
    void Shutdown();

protected:
    virtual void FixedUpdate(double deltaTime);
    virtual void FrameUpdate(double deltaTime);
    void Render();

    AssetManager _assetManager;
    InputManager _inputManager;
    RegistryManager _registryManager{};
    
    Camera _camera;
    Window _window;

private:
    void ProcessEvents();

    //Providers
    SDLAssetProvider _assetProvider;
    SDLInputProvider _inputProvider;
    SDLLoggingProvider _loggingProvider;
    SDLRenderProvider _renderProvider;
    SDLSystemTimerProvider _systemTimerProvider;

    Config _config;


    RenderSystem _renderSystem;
    MovementSystem _movementSystem;

    bool _isRunning = false;
};

//Utility Classes

class TimeTracker
{
public:
    TimeTracker(uint64_t  initialTime, uint64_t  frequency) :
        _time(initialTime), _frequency(frequency) {}

    double GetDeltaTime(uint64_t  currentTime);

private:
    static constexpr double MAX_DELTA_TIME = 0.05;

    uint64_t  _time = 0;
    uint64_t  _frequency = 0;
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
    FPSMeter(uint64_t  initialTime, uint64_t  frequency) :
        _time(initialTime), _frequency(frequency) {}

    bool Update(uint64_t  currentTime);
    int GetFPS() const { return _fps; }

private:
    static const int UPDATE_TIME = 1;

    int _fps = 0;

    uint64_t  _time = 0;
    uint64_t  _frequency = 0;
    int _frames = 0;
};