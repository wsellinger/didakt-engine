#pragma once

#include "../interfaces/IInputDriver.h"

class SDLInputDriver : public IInputDriver
{
public:
	std::array<bool, static_cast<size_t>(Key::Count)> GetKeyStates() const override;
	std::array<bool, static_cast<size_t>(MouseButton::Count)> GetMouseButtonStates() const override;
	glm::vec2 GetMousePosition() const override;
};