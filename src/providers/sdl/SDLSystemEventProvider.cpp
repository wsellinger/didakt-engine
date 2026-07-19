#include "SDLSystemEventProvider.h"

#include <SDL_events.h>

void SDLSystemEventProvider::PollEvents()
{
    _quitRequested = false;

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            _quitRequested = true;
    }
}

bool SDLSystemEventProvider::QuitRequested() const
{
    return _quitRequested;
}