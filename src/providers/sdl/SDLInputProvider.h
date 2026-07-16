#pragma once

#include "../interfaces/IInputProvider.h"

class SDLInputProvider : public IInputProvider
{
public:
	std::array<bool, static_cast<size_t>(Key::Count)> GetKeyStates() const override;
	std::array<bool, static_cast<size_t>(MouseButton::Count)> GetMouseButtonStates() const override;
	glm::vec2 GetMousePosition() const override;
};