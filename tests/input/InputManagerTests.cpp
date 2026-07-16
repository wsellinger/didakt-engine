#include "input/InputManager.h"
#include "providers/interfaces/IInputProvider.h"

#include <catch2/catch_test_macros.hpp>

namespace
{
    class MockInputProvider : public IInputProvider
    {
    public:
        std::array<bool, static_cast<size_t>(Key::Count)> GetKeyStates() const override { return keyStates; }
        std::array<bool, static_cast<size_t>(MouseButton::Count)> GetMouseButtonStates() const override { return mouseButtonStates; }
        glm::vec2 GetMousePosition() const override { return mousePosition; }

        std::array<bool, static_cast<size_t>(Key::Count)> keyStates{};
        std::array<bool, static_cast<size_t>(MouseButton::Count)> mouseButtonStates{};
        glm::vec2 mousePosition{};
    };
}

TEST_CASE("Input.InputManager.KeyPressedOnlyOnTransitionFrame", "[InputManager]")
{
    MockInputProvider provider;
    InputManager inputManager;
    inputManager.Initialize(provider);

    inputManager.Update();
    REQUIRE_FALSE(inputManager.IsKeyPressed(Key::W));

    provider.keyStates[static_cast<size_t>(Key::W)] = true;
    inputManager.Update();
    REQUIRE(inputManager.IsKeyPressed(Key::W));

    inputManager.Update();
    REQUIRE_FALSE(inputManager.IsKeyPressed(Key::W));
}

TEST_CASE("Input.InputManager.KeyDownReflectsCurrentState", "[InputManager]")
{
    MockInputProvider provider;
    InputManager inputManager;
    inputManager.Initialize(provider);

    provider.keyStates[static_cast<size_t>(Key::A)] = true;
    inputManager.Update();

    REQUIRE(inputManager.IsKeyDown(Key::A));
    REQUIRE_FALSE(inputManager.IsKeyDown(Key::B));
}

TEST_CASE("Input.InputManager.KeyReleasedOnlyOnTransitionFrame", "[InputManager]")
{
    MockInputProvider provider;
    InputManager inputManager;
    inputManager.Initialize(provider);

    provider.keyStates[static_cast<size_t>(Key::W)] = true;
    inputManager.Update(); // held

    provider.keyStates[static_cast<size_t>(Key::W)] = false;
    inputManager.Update(); // just released
    REQUIRE(inputManager.IsKeyReleased(Key::W));

    inputManager.Update(); // still up, no longer "just released"
    REQUIRE_FALSE(inputManager.IsKeyReleased(Key::W));
}

TEST_CASE("Input.InputManager.KeyNotDownBeforeFirstUpdate", "[InputManager]")
{
    MockInputProvider provider;
    InputManager inputManager;
    inputManager.Initialize(provider);

    // No Update() called yet — should reflect the safe zero-initialized default
    REQUIRE_FALSE(inputManager.IsKeyDown(Key::W));
}

TEST_CASE("Input.InputManager.MouseButtonPressedOnlyOnTransitionFrame", "[InputManager]")
{
    MockInputProvider provider;
    InputManager inputManager;
    inputManager.Initialize(provider);

    inputManager.Update();
    REQUIRE_FALSE(inputManager.IsMouseButtonPressed(MouseButton::Left));

    provider.mouseButtonStates[static_cast<size_t>(MouseButton::Left)] = true;
    inputManager.Update();
    REQUIRE(inputManager.IsMouseButtonPressed(MouseButton::Left));

    inputManager.Update();
    REQUIRE_FALSE(inputManager.IsMouseButtonPressed(MouseButton::Left));
}

TEST_CASE("Input.InputManager.MouseButtonReleasedOnlyOnTransitionFrame", "[InputManager]")
{
    MockInputProvider provider;
    InputManager inputManager;
    inputManager.Initialize(provider);

    provider.mouseButtonStates[static_cast<size_t>(MouseButton::Right)] = true;
    inputManager.Update();

    provider.mouseButtonStates[static_cast<size_t>(MouseButton::Right)] = false;
    inputManager.Update();
    REQUIRE(inputManager.IsMouseButtonReleased(MouseButton::Right));

    inputManager.Update();
    REQUIRE_FALSE(inputManager.IsMouseButtonReleased(MouseButton::Right));
}

TEST_CASE("Input.InputManager.GetMousePositionReflectsProvider", "[InputManager]")
{
    MockInputProvider provider;
    InputManager inputManager;
    inputManager.Initialize(provider);

    provider.mousePosition = { 123.0f, 456.0f };
    inputManager.Update();

    glm::vec2 position = inputManager.GetMousePosition();
    REQUIRE(position.x == 123.0f);
    REQUIRE(position.y == 456.0f);
}

TEST_CASE("Input.InputManager.KeysAreIndependent", "[InputManager]")
{
    MockInputProvider provider;
    InputManager inputManager;
    inputManager.Initialize(provider);

    provider.keyStates[static_cast<size_t>(Key::W)] = true;
    inputManager.Update();

    REQUIRE(inputManager.IsKeyDown(Key::W));
    REQUIRE_FALSE(inputManager.IsKeyDown(Key::A));
    REQUIRE_FALSE(inputManager.IsKeyDown(Key::S));
    REQUIRE_FALSE(inputManager.IsKeyDown(Key::D));
}