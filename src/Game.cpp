#include "Game.h"
#include <SDL.h>

//=== Public ===

bool Game::Initialize()
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

void Game::Run()
{
    while (_isRunning)
    {
        ProcessInput();
        Render();
    }
}

void Game::Shutdown()
{
    _window.Shutdown();
    SDL_Quit();
}

//=== Private ===

void Game::ProcessInput()
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

void Game::Render()
{
    SDL_Renderer* renderer = _window.GetSDLRenderer();
    RenderColor& color = _config.renderer.clearColor;
    SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, color.A);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}
