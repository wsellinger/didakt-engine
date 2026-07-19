#include "SDLWindowProvider.h"

#include <SDL_video.h>

void SDLWindowProvider::SetTitle(const std::string& title)
{
    SDL_SetWindowTitle(_window, title.c_str());
}
