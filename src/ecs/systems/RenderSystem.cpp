#include "RenderSystem.h"

#include "../../render/AssetManager.h"
#include "../../render/Camera.h"
#include "../components/RenderComponent.h"
#include "../components/SpriteSheetComponent.h"
#include "../components/TilemapComponent.h"
#include "../components/TransformComponent.h"

#include <algorithm>
#include <climits>
#include <vector>

#include <entt/entity/fwd.hpp>
#include <entt/entity/registry.hpp>

#include <SDL_assert.h>
#include <SDL_log.h>
#include <SDL_rect.h>
#include <SDL_render.h>

#include <glm/ext/vector_float2.hpp>

void RenderSystem::Render(entt::registry& registry, SDL_Renderer* renderer, AssetManager& assetManager, const Camera& camera)
{
    //Get Commands
    std::vector<DrawCommand> drawCommands;
    AppendCommands<SpriteSheetComponent, TransformComponent>(DrawType::Sprite, registry, drawCommands);
    AppendCommands<TilemapComponent>(DrawType::Tilemap, registry, drawCommands);

    //Sort
    std::sort(drawCommands.begin(), drawCommands.end(), 
        [](const DrawCommand& a, const DrawCommand& b) { return a.zIndex < b.zIndex; });

    //Render
    RenderParameters renderParameters{ registry, renderer, assetManager, camera };
    for (auto drawCommand : drawCommands)
    {
        switch (drawCommand.type)
        {
        case DrawType::Sprite:
            RenderSprite(drawCommand.entity, renderParameters);
            break;
        case DrawType::Tilemap:
            RenderTilemap(drawCommand.entity, renderParameters);
            break;
        default:
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "RenderSystem: unknown DrawType %d", static_cast<int>(drawCommand.type));
            break;
        }
    }
}

template<typename... Components>
void RenderSystem::AppendCommands(DrawType type, entt::registry& registry, std::vector<DrawCommand>& drawCommands)
{
    auto view = registry.view<RenderComponent, Components...>();
    for (auto entity : view)
        drawCommands.push_back({ entity, type, view.get<RenderComponent>(entity).zIndex });
}

void RenderSystem::RenderSprite(entt::entity entity, const RenderParameters& renderParameters)
{
    auto& [registry, renderer, assetManager, camera] = renderParameters;

    //Get Components
    auto view = registry.view<RenderComponent, SpriteSheetComponent, TransformComponent>();
    const auto& render = view.get<RenderComponent>(entity);
    const auto& sprite = view.get<SpriteSheetComponent>(entity);
    const auto& transform = view.get<TransformComponent>(entity);

    //Texture
    SDL_Texture* texture = assetManager.GetTexture(render.textureId);
    SDL_assert(texture != nullptr);

    //Source
    SDL_Rect source{ sprite.x, sprite.y, sprite.width, sprite.height };

    glm::vec2 worldPos{ transform.position.x, transform.position.y };
    glm::vec2 screenPos = camera.WorldToScreen(worldPos);

    //Destination
    SDL_Rect destination
    {
        static_cast<int>(screenPos.x),
        static_cast<int>(screenPos.y),
        static_cast<int>(sprite.width * transform.scale.x * camera.zoom),
        static_cast<int>(sprite.height * transform.scale.y * camera.zoom)
    };

    //Render
    SDL_RenderCopyEx(renderer, texture, &source, &destination, transform.rotation, nullptr, SDL_FLIP_NONE);
}

using TileRow = std::vector<int>;
using TileGrid = std::vector<TileRow>;

void RenderSystem::RenderTilemap(entt::entity entity, const RenderParameters& renderParameters)
{
    auto& [registry, renderer, assetManager, camera] = renderParameters;

    //Get Components
    auto view = registry.view<RenderComponent, TilemapComponent>();
    const auto& render = view.get<RenderComponent>(entity);
    const auto& tilemap = view.get<TilemapComponent>(entity);

    //Texture
    SDL_Texture* texture = assetManager.GetTexture(render.textureId);
    SDL_assert(texture != nullptr);
    
    //Grid
    const TileGrid& tiles = tilemap.tiles;
    size_t numRows = tiles.size();

    //Tiles
    for (size_t iRow = 0; iRow < numRows; iRow++)
    {
        const TileRow& row = tiles[iRow];
        size_t numCol = row.size();

        for (size_t iCol = 0; iCol < numCol; iCol++)
        {
            int id = row[iCol];
            SDL_Rect source = GetSourceRect(id, tilemap);
            SDL_Rect destination = GetDestinationRect(iRow, iCol, tilemap, camera);

            SDL_RenderCopy(renderer, texture, &source, &destination);
        }
    }
}

SDL_Rect RenderSystem::GetSourceRect(int id, const TilemapComponent& tilemap)
{
    SDL_assert(id >= 0);
    SDL_assert(tilemap.sheetColumns > 0);

    return
    {
        (id % tilemap.sheetColumns) * tilemap.tileWidth,
        (id / tilemap.sheetColumns) * tilemap.tileHeight,
        tilemap.tileWidth,
        tilemap.tileHeight
    };
}

SDL_Rect RenderSystem::GetDestinationRect(size_t iRow, size_t iCol, const TilemapComponent& tilemap, const Camera& camera)
{
    SDL_assert(iCol < INT_MAX);
    SDL_assert(iRow < INT_MAX);
    SDL_assert(tilemap.tileWidth > 0);
    SDL_assert(tilemap.tileHeight > 0);
    SDL_assert(camera.zoom > 0);

    float fCol = static_cast<float>(iCol);
    float fRow = static_cast<float>(iRow);
    float fWidth = static_cast<float>(tilemap.tileWidth);
    float fHeight = static_cast<float>(tilemap.tileHeight);

    glm::vec2 worldPos{ fCol * fWidth, fRow * fHeight };
    glm::vec2 screenPos = camera.WorldToScreen(worldPos);

    float w = fWidth * camera.zoom;
    float h = fHeight * camera.zoom;

    SDL_assert(screenPos.x >= INT_MIN && screenPos.x <= INT_MAX);
    SDL_assert(screenPos.y >= INT_MIN && screenPos.y <= INT_MAX);
    SDL_assert(w >= INT_MIN && w <= INT_MAX);
    SDL_assert(h >= INT_MIN && h <= INT_MAX);

    return { static_cast<int>(screenPos.x), static_cast<int>(screenPos.y), static_cast<int>(w), static_cast<int>(h) };
}