#include "SDLProviderManager.h"

#include <SDL.h>
#include <SDL_image.h>

bool SDLProviderManager::Initialize(Config config)
{
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

    //Create Window
    {
        auto& [title, width, height] = config.window;
        _window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
        if (!_window)
            return false;
    }

    //Create Renderer
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!_renderer)
        return false;

    //Init Providers
    _windowProvider.Initialize(_window);
    _assetProvider.Initialize(_renderer);
    _renderProvider.Initialize(_renderer);

    return true;
}

void SDLProviderManager::Shutdown()
{
    IMG_Quit();

    if (_renderer)
        SDL_DestroyRenderer(_renderer);

    if (_window)
        SDL_DestroyWindow(_window);

    SDL_Quit();
}

