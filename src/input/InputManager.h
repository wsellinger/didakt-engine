#pragma once

#include <array>

#include <SDL_scancode.h>
#include <SDL_stdinc.h>

#include <glm/ext/vector_float2.hpp>

class InputManager
{
public:
	InputManager() = default;
    ~InputManager() = default;

    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;
    InputManager(InputManager&&) = delete;
    InputManager& operator=(InputManager&&) = delete;

    void Update();

    bool IsKeyDown(SDL_Scancode key) const;
    bool IsKeyPressed(SDL_Scancode key) const;
    bool IsKeyReleased(SDL_Scancode key) const;

    bool IsMouseButtonDown(Uint32 button) const;
    bool IsMouseButtonPressed(Uint32 button) const;
    bool IsMouseButtonReleased(Uint32 button) const;

    glm::vec2 GetMousePosition() const;

private:

    //Keyboard

    std::array<Uint8, SDL_NUM_SCANCODES> _currentKeyStates{};
    std::array<Uint8, SDL_NUM_SCANCODES> _previousKeyStates{};
    
    //Mouse

    Uint32 _currentMouseButtonStates{};
    Uint32 _previousMouseButtonStates{};
    Sint32 _mouseX{};
    Sint32 _mouseY{};

    Uint32 GetMouseButtonState(Uint32 button, Uint32 states) const;
};
