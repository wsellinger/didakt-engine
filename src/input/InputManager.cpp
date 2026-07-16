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
    _currentKeyStates = _provider->GetKeyStates();
    _currentMouseButtonStates = _provider->GetMouseButtonStates();
    _currentMousePosition = _provider->GetMousePosition();
}

//Keyboard

bool InputManager::IsKeyDown(Key key) const
{
    return _currentKeyStates[static_cast<size_t>(key)] != 0;
}

bool InputManager::IsKeyPressed(Key key) const
{    
    return IsKeyDown(key) && _previousKeyStates[static_cast<size_t>(key)] == 0;
}

bool InputManager::IsKeyReleased(Key key) const
{    
    return !IsKeyDown(key) && _previousKeyStates[static_cast<size_t>(key)] != 0;
}

//Mouse

bool InputManager::IsMouseButtonDown(MouseButton button) const
{
    return _currentMouseButtonStates[static_cast<size_t>(button)] != 0;
}

bool InputManager::IsMouseButtonPressed(MouseButton button) const
{
    return IsMouseButtonDown(button) && _previousMouseButtonStates[static_cast<size_t>(button)] == 0;
}

bool InputManager::IsMouseButtonReleased(MouseButton button) const
{
    return !IsMouseButtonDown(button) && _previousMouseButtonStates[static_cast<size_t>(button)] != 0;
}