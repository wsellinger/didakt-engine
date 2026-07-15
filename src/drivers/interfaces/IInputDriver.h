#pragma once

#include "../../input/Key.h"
#include "../../input/MouseButton.h"

#include <array>

#include <glm/ext/vector_float2.hpp>

class IInputDriver
{
public:
    virtual ~IInputDriver() = default;
    virtual std::array<bool, static_cast<size_t>(Key::Count)> GetKeyStates() const = 0; 
    virtual std::array<bool, static_cast<size_t>(MouseButton::Count)> GetMouseButtonStates() const = 0;
    virtual glm::vec2 GetMousePosition() const = 0;
};