#pragma once
#include <SDL.h>
#include <string>
#include <SDL_render.h>
#include <SDL_video.h>

class Window
{
public:
    Window() = default;
    ~Window() = default;

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    bool Initialize(const std::string& title, int width, int height);
    void Shutdown();

    SDL_Window* GetWindow() const { return _window; }
    SDL_Renderer* GetRenderer() const { return _renderer; }

private:
    SDL_Window* _window = nullptr;
    SDL_Renderer* _renderer = nullptr;
};