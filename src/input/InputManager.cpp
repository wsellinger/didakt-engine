#include "InputManager.h"

#include <algorithm>

#include <SDL_keyboard.h>
#include <SDL_mouse.h>
#include <SDL_scancode.h>
#include <SDL_stdinc.h>

#include <glm/ext/vector_float2.hpp>

void InputManager::Update()
{
    //Save States
    _previousKeyStates = _currentKeyStates;
    _previousMouseButtonStates = _currentMouseButtonStates;

    //Update States
    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
    std::copy(keyboardState, keyboardState + SDL_NUM_SCANCODES, _currentKeyStates.begin());

    _currentMouseButtonStates = SDL_GetMouseState(&_mouseX, &_mouseY);
}

//Keyboard

bool InputManager::IsKeyDown(SDL_Scancode key) const
{
    return _currentKeyStates[key] != 0;
}

bool InputManager::IsKeyPressed(SDL_Scancode key) const
{    
    return IsKeyDown(key) && _previousKeyStates[key] == 0;
}

bool InputManager::IsKeyReleased(SDL_Scancode key) const
{    
    return !IsKeyDown(key) && _previousKeyStates[key] != 0;
}

//Mouse

bool InputManager::IsMouseButtonDown(Uint32 button) const
{
    return GetMouseButtonState(button, _currentMouseButtonStates) != 0;
}

bool InputManager::IsMouseButtonPressed(Uint32 button) const
{
    return IsMouseButtonDown(button) && GetMouseButtonState(button, _previousMouseButtonStates) == 0;
}

bool InputManager::IsMouseButtonReleased(Uint32 button) const
{
    return !IsMouseButtonDown(button) && GetMouseButtonState(button, _previousMouseButtonStates) != 0;
}

glm::vec2 InputManager::GetMousePosition() const
{
    return { static_cast<float>(_mouseX), static_cast<float>(_mouseY) };
}

//Private

Uint32 InputManager::GetMouseButtonState(Uint32 button, Uint32 states) const
{
    return states & SDL_BUTTON(button);
}
