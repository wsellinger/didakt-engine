#include "Engine.h"

#include "Config.h"

#include <string>

//#include <begin_code.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_stdinc.h>
#include <SDL_timer.h>
#include <SDL_video.h>

using string = std::string;

//=== Public ===

bool Engine::Initialize()
{
    //Load Config
    _config = LoadConfig(Config::DEFAULT_PATH);

    //Init SDL
    {
        unsigned int flags = SDL_INIT_VIDEO;
        DECLSPEC int result = SDL_Init(flags);
        if (result != 0)
            return false;
    }

    //Init SDL Image
    {
        unsigned int flags = IMG_INIT_PNG || IMG_INIT_JPG;
        int result = IMG_Init(flags);
        if (result != flags)
            return false;
    }

    //Init Window
    {
        WindowConfig& config = _config.window;
        bool isInit = _window.Initialize(config.title, config.width, config.height);
        if (!isInit)
            return false;
    }

    _isRunning = true;

    return true;
}

void Engine::Run()
{
    //Init Time
    Uint64 initialTime = SDL_GetPerformanceCounter();
    Uint64 frequency = SDL_GetPerformanceFrequency();

    TimeTracker timeTracker(initialTime, frequency);
    FixedTimer fixedTimer;
    FPSMeter fpsMeter(initialTime, frequency);

    //Main Loop
    while (_isRunning)
    {
        //Update Time
        Uint64 currentTime = SDL_GetPerformanceCounter();
        double deltaTime = timeTracker.GetDeltaTime(currentTime);
        
        //Fixed Update
        fixedTimer.Increment(deltaTime);
        while (fixedTimer.Update())
        {
            FixedUpdate(fixedTimer.GetTimestep());
        }

        FrameUpdate(deltaTime);

        //FPS Update
        if (fpsMeter.Update(currentTime))
        {
            string title = _config.window.title + " | FPS: " + std::to_string(fpsMeter.GetFPS());
            SDL_SetWindowTitle(_window.GetWindow(), title.c_str());
        }
    }
}

void Engine::Shutdown()
{
    _assetManager.ClearAll();
    IMG_Quit();

    _window.Shutdown();
    SDL_Quit();
}

//=== Private ===

void Engine::ProcessInput()
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
    //TODO Implement
}

void Engine::FrameUpdate(double deltaTime)
{
    ProcessInput();
    Render();
}

void Engine::Render()
{
    SDL_Renderer* renderer = _window.GetRenderer();
    RenderColor& color = _config.renderer.clearColor;
    SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, color.A);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}


//=== TimeTracker ===

double TimeTracker::GetDeltaTime(Uint64 currentTime)
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

bool FPSMeter::Update(Uint64 currentTime)
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
