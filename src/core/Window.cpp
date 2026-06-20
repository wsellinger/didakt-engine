#include "Window.h"
#include <SDL_render.h>
#include <SDL_video.h>

bool Window::Initialize(const std::string& title, int width, int height)
{
    //Init Window
    _window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (!_window)
        return false;

    //Init Renderer
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!_renderer)
        return false;

    return true;
}

void Window::Shutdown()
{
    //Destroy Private Members
    if (_renderer)
        SDL_DestroyRenderer(_renderer); 


    if (_window)
        SDL_DestroyWindow(_window);
}