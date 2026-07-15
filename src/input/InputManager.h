#pragma once

#include "../drivers/interfaces/IInputDriver.h"
#include "key.h"
#include "MouseButton.h"

#include <array>

#include <glm/ext/vector_float2.hpp>

class InputManager
{
public:
	explicit InputManager(IInputDriver& driver) : _driver(driver) {}
    ~InputManager() = default;

    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;
    InputManager(InputManager&&) = delete;
    InputManager& operator=(InputManager&&) = delete;

    void Update();

    bool IsKeyDown(Key key) const;
    bool IsKeyPressed(Key key) const;
    bool IsKeyReleased(Key key) const;

    bool IsMouseButtonDown(MouseButton button) const;
    bool IsMouseButtonPressed(MouseButton button) const;
    bool IsMouseButtonReleased(MouseButton button) const;

    glm::vec2 GetMousePosition() const { return _currentMousePosition; }

private:

    IInputDriver& _driver;

    //Keyboard

    std::array<bool, static_cast<size_t>(Key::Count)> _currentKeyStates{};
    std::array<bool, static_cast<size_t>(Key::Count)> _previousKeyStates{};
    
    //Mouse

    std::array<bool, static_cast<size_t>(MouseButton::Count)> _currentMouseButtonStates{};
    std::array<bool, static_cast<size_t>(MouseButton::Count)> _previousMouseButtonStates{};
    glm::vec2 _currentMousePosition{};    
};
