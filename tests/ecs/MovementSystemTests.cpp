#include "ecs/systems/MovementSystem.h"
#include "ecs/components/TransformComponent.h"
#include "ecs/components/RigidBodyComponent.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <entt/entity/registry.hpp>
#include <glm/ext/vector_float2.hpp>

TEST_CASE("ECS.MovementSystem.AppliesVelocityOverTime", "[ECS][System][Movement]")
{
    entt::registry registry;
    auto entity = registry.create();
    registry.emplace<TransformComponent>(entity, glm::vec2{ 0.0f, 0.0f }, 0.0f, glm::vec2{ 1.0f, 1.0f });
    registry.emplace<RigidBodyComponent>(entity, glm::vec2{ 100.0f, 0.0f });

    MovementSystem movementSystem;
    movementSystem.Update(registry, 0.5); // half a second

    const auto& transform = registry.get<TransformComponent>(entity);
    REQUIRE(transform.position.x == Catch::Approx(50.0f));
    REQUIRE(transform.position.y == Catch::Approx(0.0f));
}

TEST_CASE("ECS.MovementSystem.ZeroVelocityDoesNotMoveEntity", "[ECS][System][Movement]")
{
    entt::registry registry;
    auto entity = registry.create();
    registry.emplace<TransformComponent>(entity, glm::vec2{ 25.0f, 75.0f }, 0.0f, glm::vec2{ 1.0f, 1.0f });
    registry.emplace<RigidBodyComponent>(entity, glm::vec2{ 0.0f, 0.0f });

    MovementSystem movementSystem;
    movementSystem.Update(registry, 1.0);

    const auto& transform = registry.get<TransformComponent>(entity);
    REQUIRE(transform.position.x == Catch::Approx(25.0f));
    REQUIRE(transform.position.y == Catch::Approx(75.0f));
}

TEST_CASE("ECS.MovementSystem.ZeroDeltaTimeDoesNotMoveEntity", "[ECS][System][Movement]")
{
    entt::registry registry;
    auto entity = registry.create();
    registry.emplace<TransformComponent>(entity, glm::vec2{ 10.0f, 10.0f }, 0.0f, glm::vec2{ 1.0f, 1.0f });
    registry.emplace<RigidBodyComponent>(entity, glm::vec2{ 500.0f, -300.0f });

    MovementSystem movementSystem;
    movementSystem.Update(registry, 0.0);

    const auto& transform = registry.get<TransformComponent>(entity);
    REQUIRE(transform.position.x == Catch::Approx(10.0f));
    REQUIRE(transform.position.y == Catch::Approx(10.0f));
}

TEST_CASE("ECS.MovementSystem.AppliesNegativeVelocity", "[ECS][System][Movement]")
{
    entt::registry registry;
    auto entity = registry.create();
    registry.emplace<TransformComponent>(entity, glm::vec2{ 100.0f, 100.0f }, 0.0f, glm::vec2{ 1.0f, 1.0f });
    registry.emplace<RigidBodyComponent>(entity, glm::vec2{ -50.0f, -25.0f });

    MovementSystem movementSystem;
    movementSystem.Update(registry, 2.0);

    const auto& transform = registry.get<TransformComponent>(entity);
    REQUIRE(transform.position.x == Catch::Approx(0.0f));   // 100 + (-50 * 2)
    REQUIRE(transform.position.y == Catch::Approx(50.0f));  // 100 + (-25 * 2)
}

TEST_CASE("ECS.MovementSystem.MovesMultipleEntitiesIndependently", "[ECS][System][Movement]")
{
    entt::registry registry;

    auto entityA = registry.create();
    registry.emplace<TransformComponent>(entityA, glm::vec2{ 0.0f, 0.0f }, 0.0f, glm::vec2{ 1.0f, 1.0f });
    registry.emplace<RigidBodyComponent>(entityA, glm::vec2{ 10.0f, 0.0f });

    auto entityB = registry.create();
    registry.emplace<TransformComponent>(entityB, glm::vec2{ 0.0f, 0.0f }, 0.0f, glm::vec2{ 1.0f, 1.0f });
    registry.emplace<RigidBodyComponent>(entityB, glm::vec2{ 0.0f, 20.0f });

    MovementSystem movementSystem;
    movementSystem.Update(registry, 1.0);

    const auto& transformA = registry.get<TransformComponent>(entityA);
    const auto& transformB = registry.get<TransformComponent>(entityB);

    REQUIRE(transformA.position.x == Catch::Approx(10.0f));
    REQUIRE(transformA.position.y == Catch::Approx(0.0f));

    REQUIRE(transformB.position.x == Catch::Approx(0.0f));
    REQUIRE(transformB.position.y == Catch::Approx(20.0f));
}

TEST_CASE("ECS.MovementSystem.IgnoresEntitiesWithoutRigidBody", "[ECS][System][Movement]")
{
    entt::registry registry;

    // Entity with only a TransformComponent — no RigidBodyComponent
    auto entity = registry.create();
    registry.emplace<TransformComponent>(entity, glm::vec2{ 5.0f, 5.0f }, 0.0f, glm::vec2{ 1.0f, 1.0f });

    MovementSystem movementSystem;
    movementSystem.Update(registry, 1.0);

    const auto& transform = registry.get<TransformComponent>(entity);
    REQUIRE(transform.position.x == Catch::Approx(5.0f));
    REQUIRE(transform.position.y == Catch::Approx(5.0f));
}