#include "Engine.h"

#include "Config.h"
#include "Logger.h"

#include <string>

#include <SDL.h>
#include <SDL_image.h>

using string = std::string;

//=== Public ===

bool Engine::Initialize()
{
    //Init Logging
    Logger::SetProvider(_loggingProvider);

    //Load Config
    _config = LoadConfig(Config::DEFAULT_PATH);

    //Init SDL
    {
        unsigned int flags = SDL_INIT_VIDEO;
        int result = SDL_Init(flags);
        if (result != 0)
            return false;
    }

    //Init SDL Image
    {
        unsigned int flags = IMG_INIT_PNG | IMG_INIT_JPG;
        int result = IMG_Init(flags);
        if (result != flags)
            return false;
    }


    //Init Window
    {
        auto& [title, width, height] = _config.window;
        _window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
        if (!_window)
            return false;
    }

    //Init Renderer
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!_renderer)
        return false;

    //Init Providers
    _windowProvider.Initialize(_window);
    _assetProvider.Initialize(_renderer);
    _renderProvider.Initialize(_renderer);

    //Init Managers
    _assetManager.Initialize(_assetProvider);
    _inputManager.Initialize(_inputProvider);

    _isRunning = true;

    return true;
}

void Engine::Run()
{
    //Init Time
    uint64_t  initialTime = _systemTimerProvider.GetTime();
    uint64_t  frequency = _systemTimerProvider.GetFrequency();

    TimeTracker timeTracker(initialTime, frequency);
    FixedTimer fixedTimer;
    FPSMeter fpsMeter(initialTime, frequency);

    //Main Loop
    while (_isRunning)
    {
        //Update Time
        uint64_t  currentTime = _systemTimerProvider.GetTime();
        double deltaTime = timeTracker.GetDeltaTime(currentTime);
        
        //Events
        ProcessEvents();

        //Input
        _inputManager.Update();

        //Fixed Update
        fixedTimer.Increment(deltaTime);
        while (fixedTimer.Update())
        {
            FixedUpdate(fixedTimer.GetTimestep());
        }

        FrameUpdate(deltaTime);

        //Render
        Render();

        //FPS Update
        if (fpsMeter.Update(currentTime))
        {
            string title = _config.window.title + " | FPS: " + std::to_string(fpsMeter.GetFPS());
            _windowProvider.SetTitle(title);
        }
    }
}

void Engine::Shutdown()
{
    _assetManager.ClearAll();
    IMG_Quit();

    if (_renderer)
        SDL_DestroyRenderer(_renderer);

    if (_window)
        SDL_DestroyWindow(_window);

    SDL_Quit();
}

//=== Private ===

void Engine::ProcessEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        //Quit
        if (event.type == SDL_QUIT)
            _isRunning = false;

        //Escape Key
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            _isRunning = false;
    }
}

void Engine::FixedUpdate(double deltaTime)
{
    _movementSystem.Update(_registryManager.GetRegistry(), deltaTime);
}

void Engine::FrameUpdate(double deltaTime)
{

}

void Engine::Render()
{
    //Render Background
    RenderColor& color = _config.renderer.clearColor;
    _renderProvider.Clear(color);
    _renderSystem.Render(_registryManager.GetRegistry(), _renderProvider, _assetManager, _camera);
    _renderProvider.Present();
}


//=== TimeTracker ===

double TimeTracker::GetDeltaTime(uint64_t  currentTime)
{
    double deltaTime = static_cast<double>(currentTime - _time) / _frequency;
    _time = currentTime;

    //Prevent Spiral
    if (deltaTime > MAX_DELTA_TIME)
        deltaTime = MAX_DELTA_TIME;

    return deltaTime;
}

//=== LogicTimer ===

bool FixedTimer::Update()
{
    if (_accumulator >= FIXED_TIMESTEP)
    {
        _accumulator -= FIXED_TIMESTEP;
        return true;
    }

    return false;
}

//=== FPSMeter ===

bool FPSMeter::Update(uint64_t  currentTime)
{
    _frames++;
    double time = static_cast<double>(currentTime - _time) / _frequency;

    if (time >= UPDATE_TIME)
    {
        _fps = static_cast<int>(_frames / time);
        _frames = 0;
        _time = currentTime;

        return true;
    }

    return false;
}
