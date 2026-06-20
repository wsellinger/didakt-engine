#pragma once

#include <glm/ext/vector_float2.hpp>

struct TransformComponent
{
    glm::vec2 position{};
    float rotation{};
    glm::vec2 scale{};
};
