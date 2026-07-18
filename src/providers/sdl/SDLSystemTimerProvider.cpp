#include "SDLSystemTimerProvider.h"

#include <SDL_timer.h>

uint64_t SDLSystemTimerProvider::GetTime() const
{
    return SDL_GetPerformanceCounter();
}

uint64_t SDLSystemTimerProvider::GetFrequency() const
{
    return SDL_GetPerformanceFrequency();
}
