#include "Engine.h"
#include <SDL.h>
#include <string>

using string = std::string;

//=== Public ===

bool Engine::Initialize()
{
    //Load Config
    _config = LoadConfig(Config::DEFAULT_PATH);

    //Init SDL
    unsigned int sdlFlags = SDL_INIT_VIDEO;
    DECLSPEC int sdlInitResult = SDL_Init(sdlFlags);
    if (sdlInitResult != 0)
        return false;

    //Init Window
    WindowConfig& windowConfig = _config.window;
    bool isWindowInit = _window.Initialize(windowConfig.title, windowConfig.width, windowConfig.height);
    if (!isWindowInit)
        return false;

    _isRunning = true;

    return true;
}

void Engine::Run()
{
    //Init Time
    Uint64 initialTime = SDL_GetPerformanceCounter();
    Uint64 frequency = SDL_GetPerformanceFrequency();

    FPSMeter fpsMeter(initialTime, frequency);

    while (_isRunning)
    {
        //Update Time
        Uint64 currentTime = SDL_GetPerformanceCounter();
        double deltaTime = static_cast<double>(currentTime - initialTime) / frequency;
        initialTime = currentTime;
        
        //Update FPS
        if (fpsMeter.Update(currentTime))
        {
            string title = _config.window.title + " | FPS: " + std::to_string(fpsMeter.GetFPS());
            SDL_SetWindowTitle(_window.GetSDLWindow(), title.c_str());
        }

        ProcessInput();
        Render();
    }
}

void Engine::Shutdown()
{
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

void Engine::Render()
{
    SDL_Renderer* renderer = _window.GetSDLRenderer();
    RenderColor& color = _config.renderer.clearColor;
    SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, color.A);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}


//=== FPS Meter ===

bool FPSMeter::Update(Uint64 currentTime)
{
    _frames++;
    double time = static_cast<double>(currentTime - _timer) / _frequency;

    if (time >= UPDATE_TIME)
    {
        _fps = static_cast<int>(_frames / time);
        _frames = 0;
        _timer = currentTime;

        return true;
    }

    return false;
}