#pragma once

#include <glm/ext/vector_float2.hpp>

struct BoxColliderComponent
{
    int width{};
    int height{};
    glm::vec2 offset{};
};