#pragma once

#include <entt/entity/fwd.hpp>

class MovementSystem
{
public:
    MovementSystem() = default;
    ~MovementSystem() = default;

    MovementSystem(const MovementSystem&) = delete;
    MovementSystem& operator=(const MovementSystem&) = delete;
    MovementSystem(MovementSystem&&) = delete;
    MovementSystem& operator=(MovementSystem&&) = delete;

    void Update(entt::registry & registry, double deltaTime);
};