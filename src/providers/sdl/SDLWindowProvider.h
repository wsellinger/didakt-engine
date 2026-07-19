#pragma once

#include "../interfaces/IWindowProvider.h"

#include <SDL_video.h>

class SDLWindowProvider : public IWindowProvider
{
public:
    void Initialize(SDL_Window* window) { _window = window; }

    void SetTitle(const std::string& title) override;

private:
    SDL_Window* _window = nullptr;
};