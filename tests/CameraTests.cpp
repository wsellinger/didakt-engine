#include "render/Camera.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <glm/ext/vector_float2.hpp>

TEST_CASE("Render.Camera.WorldToScreen_NoOp", "[Render][Camera]")
{
    Camera camera{ 0.0f, 0.0f, 1.0f };

    glm::vec2 worldPos{ 100.0f, 50.0f };
    glm::vec2 screenPos = camera.WorldToScreen(worldPos);

    REQUIRE(screenPos.x == Catch::Approx(worldPos.x));
    REQUIRE(screenPos.y == Catch::Approx(worldPos.y));
}

TEST_CASE("Render.Camera.WorldToScreen_AppliesPositionOffset", "[Render][Camera]")
{
    Camera camera{ 50.0f, 30.0f, 1.0f };

    glm::vec2 worldPos{ 100.0f, 100.0f };
    glm::vec2 screenPos = camera.WorldToScreen(worldPos);

    REQUIRE(screenPos.x == Catch::Approx(50.0f));  // 100 - 50
    REQUIRE(screenPos.y == Catch::Approx(70.0f));  // 100 - 30
}

TEST_CASE("Render.Camera.WorldToScreen_AppliesZoom", "[Render][Camera]")
{
    Camera camera{ 0.0f, 0.0f, 2.0f };

    glm::vec2 worldPos{ 100.0f, 50.0f };
    glm::vec2 screenPos = camera.WorldToScreen(worldPos);

    REQUIRE(screenPos.x == Catch::Approx(200.0f));
    REQUIRE(screenPos.y == Catch::Approx(100.0f));
}

TEST_CASE("Render.Camera.WorldToScreen_AppliesOffsetThenZoom", "[Render][Camera]")
{
    Camera camera{ 10.0f, 10.0f, 2.0f };

    glm::vec2 worldPos{ 20.0f, 30.0f };
    glm::vec2 screenPos = camera.WorldToScreen(worldPos);

    // (20 - 10) * 2 = 20, (30 - 10) * 2 = 40
    REQUIRE(screenPos.x == Catch::Approx(20.0f));
    REQUIRE(screenPos.y == Catch::Approx(40.0f));
}

TEST_CASE("Render.Camera.ScreenToWorld_NoOp", "[Render][Camera]")
{
    Camera camera{ 0.0f, 0.0f, 1.0f };

    glm::vec2 screenPos{ 100.0f, 50.0f };
    glm::vec2 worldPos = camera.ScreenToWorld(screenPos);

    REQUIRE(worldPos.x == Catch::Approx(screenPos.x));
    REQUIRE(worldPos.y == Catch::Approx(screenPos.y));
}

TEST_CASE("Render.Camera.ScreenToWorld_IsInverseOfWorldToScreen", "[Render][Camera]")
{
    Camera camera{ 50.0f, 30.0f, 2.0f };

    glm::vec2 original{ 123.0f, 456.0f };
    glm::vec2 screen = camera.WorldToScreen(original);
    glm::vec2 roundTrip = camera.ScreenToWorld(screen);

    REQUIRE(roundTrip.x == Catch::Approx(original.x));
    REQUIRE(roundTrip.y == Catch::Approx(original.y));
}

TEST_CASE("Render.Camera.WorldToScreen_IsInverseOfScreenToWorld", "[Render][Camera]")
{
    Camera camera{ -25.0f, 75.0f, 0.5f };

    glm::vec2 original{ 300.0f, -150.0f };
    glm::vec2 world = camera.ScreenToWorld(original);
    glm::vec2 roundTrip = camera.WorldToScreen(world);

    REQUIRE(roundTrip.x == Catch::Approx(original.x));
    REQUIRE(roundTrip.y == Catch::Approx(original.y));
}

TEST_CASE("Render.Camera.WorldToScreen_NegativeCameraPosition", "[Render][Camera]")
{
    Camera camera{ -50.0f, -50.0f, 1.0f };

    glm::vec2 worldPos{ 0.0f, 0.0f };
    glm::vec2 screenPos = camera.WorldToScreen(worldPos);

    REQUIRE(screenPos.x == Catch::Approx(50.0f));
    REQUIRE(screenPos.y == Catch::Approx(50.0f));
}