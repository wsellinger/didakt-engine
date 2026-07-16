#include "render/RenderMath.h"

#include "render/Camera.h"
#include "render/RenderRect.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Render.RenderMath.GetSourceRect_ComputesCorrectGridCell", "[RenderMath]")
{
    int width = 32;
    int height = 32;
    int columns = 4;

    RenderRect rect = RenderMath::GetTileSourceRect(5, width, height, columns); // row 1, col 1

    REQUIRE(rect.x == 32);
    REQUIRE(rect.y == 32);
    REQUIRE(rect.w == 32);
    REQUIRE(rect.h == 32);
}

TEST_CASE("Render.RenderMath.GetSourceRect_FirstTileIsTopLeft", "[RenderMath]")
{
    int width = 16;
    int height = 16;
    int columns = 8;

    RenderRect rect = RenderMath::GetTileSourceRect(0, width, height, columns);

    REQUIRE(rect.x == 0);
    REQUIRE(rect.y == 0);
}

TEST_CASE("Render.RenderMath.GetDestinationRect_NoZoomMatchesWorldGrid", "[RenderMath]")
{
    int tileWidth = 32;
    int tileHeight = 32;

    Camera camera{ 0.0f, 0.0f, 1.0f };

    RenderRect rect = RenderMath::GetTileDestinationRect(2, 3, tileWidth, tileHeight, camera); // row 2, col 3

    REQUIRE(rect.x == 96);  // col 3 * 32
    REQUIRE(rect.y == 64);  // row 2 * 32
    REQUIRE(rect.w == 32);
    REQUIRE(rect.h == 32);
}

TEST_CASE("Render.RenderMath.GetDestinationRect_AppliesZoom", "[RenderMath]")
{
    int tileWidth = 32;
    int tileHeight = 32;

    Camera camera{ 0.0f, 0.0f, 2.0f };

    RenderRect rect = RenderMath::GetTileDestinationRect(0, 0, tileWidth, tileHeight, camera);

    REQUIRE(rect.w == 64); // 32 * 2
    REQUIRE(rect.h == 64);
}

TEST_CASE("Render.RenderMath.GetDestinationRect_AdjacentTilesShareExactBoundary", "[RenderMath]")
{
    // Regression test for the seam bug, tile N's right edge must exactly
    // equal tile N+1's left edge, at a zoom level prone to rounding drift.
    int tileWidth = 37; // deliberately awkward size
    int tileHeight = 37;

    Camera camera{ 0.0f, 0.0f, 1.37f };

    RenderRect tileA = RenderMath::GetTileDestinationRect(0, 0, tileWidth, tileHeight, camera);
    RenderRect tileB = RenderMath::GetTileDestinationRect(0, 1, tileWidth, tileHeight, camera);

    REQUIRE(tileA.x + tileA.w == tileB.x);
}

TEST_CASE("Render.RenderMath.GetSpriteDestinationRect_NoZoomMatchesWorldPosition", "[RenderMath]")
{
    Camera camera{ 0.0f, 0.0f, 1.0f };

    RenderRect rect = RenderMath::GetSpriteDestinationRect(
        glm::vec2{ 100.0f, 50.0f },
        glm::vec2{ 32.0f, 32.0f },
        camera
    );

    REQUIRE(rect.x == 100);
    REQUIRE(rect.y == 50);
    REQUIRE(rect.w == 32);
    REQUIRE(rect.h == 32);
}

TEST_CASE("Render.RenderMath.GetSpriteDestinationRect_AppliesZoomAndScale", "[RenderMath]")
{
    Camera camera{ 0.0f, 0.0f, 2.0f };

    RenderRect rect = RenderMath::GetSpriteDestinationRect(
        glm::vec2{ 0.0f, 0.0f },
        glm::vec2{ 32.0f, 32.0f }, // e.g. sprite.width * transform.scale.x already applied
        camera
    );

    REQUIRE(rect.w == 64);
    REQUIRE(rect.h == 64);
}

TEST_CASE("Render.RenderMath.GetSpriteDestinationRect_AdjacentSpritesShareExactBoundary", "[RenderMath]")
{
    Camera camera{ 0.0f, 0.0f, 1.37f };

    glm::vec2 spriteSize{ 37.0f, 37.0f };

    RenderRect spriteA = RenderMath::GetSpriteDestinationRect(glm::vec2{ 0.0f, 0.0f }, spriteSize, camera);
    RenderRect spriteB = RenderMath::GetSpriteDestinationRect(glm::vec2{ 37.0f, 0.0f }, spriteSize, camera);

    REQUIRE(spriteA.x + spriteA.w == spriteB.x);
}