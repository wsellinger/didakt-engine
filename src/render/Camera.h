#pragma once

#include <glm/ext/vector_float2.hpp>

struct Camera
{
    float x{ 0.0f };
    float y{ 0.0f };
    float zoom{ 1.0f };

    glm::vec2 ScreenToWorld(glm::vec2 screenPos) const
    {
        return (screenPos / zoom) + glm::vec2(x, y);
    }

    glm::vec2 WorldToScreen(glm::vec2 worldPos) const
    {
        return (worldPos - glm::vec2(x, y)) * zoom;
    }
};