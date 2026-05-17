#pragma once
#include <SDL.h>
#include <string>

class Window
{
public:
    bool Initialize(const std::string& title, int width, int height);
    void Shutdown();

    SDL_Window* GetSDLWindow() const { return _window; }
    SDL_Renderer* GetSDLRenderer() const { return _renderer; }
private:
    SDL_Window* _window = nullptr;
    SDL_Renderer* _renderer = nullptr;
};