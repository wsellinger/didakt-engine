#include "Game.h"
#include <SDL.h>

//=== Public ===

bool Game::Initialize()
{
    //Init SDL
    DECLSPEC int sdlInitResult = SDL_Init(SDL_INIT_VIDEO);
    if (sdlInitResult != 0)
        return false;

    //Init Window
    bool isWindowInit = _window.Initialize("Didakt", 800, 600);
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}
