#include "RenderSystem.h"

#include "../../render/AssetManager.h"
#include "../../render/Camera.h"
#include "../components/RenderComponent.h"
#include "../components/SpriteSheetComponent.h"
#include "../components/TilemapComponent.h"
#include "../components/TransformComponent.h"

#include <algorithm>
#include <climits>
#include <cmath>
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

    //Destination
    glm::vec2 worldSize{ sprite.width * transform.scale.x, sprite.height * transform.scale.y };
    glm::vec2 topLeft = camera.WorldToScreen(transform.position);
    glm::vec2 bottomRight = camera.WorldToScreen(transform.position + worldSize);

    int xtl = static_cast<int>(std::round(topLeft.x));
    int ytl = static_cast<int>(std::round(topLeft.y));
    int xbr = static_cast<int>(std::round(bottomRight.x));
    int ybr = static_cast<int>(std::round(bottomRight.y));

    SDL_Rect destination{ xtl, ytl, xbr - xtl, ybr - ytl };

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

    glm::vec2 topLeft = camera.WorldToScreen({ fCol * fWidth, fRow * fHeight });
    glm::vec2 bottomRight = camera.WorldToScreen({ (fCol + 1) * fWidth, (fRow + 1) * fHeight });

    int xtl = static_cast<int>(std::round(topLeft.x));
    int ytl = static_cast<int>(std::round(topLeft.y));
    int xbr = static_cast<int>(std::round(bottomRight.x));
    int ybr = static_cast<int>(std::round(bottomRight.y));

    SDL_assert(xtl >= INT_MIN && xtl <= INT_MAX);
    SDL_assert(ytl >= INT_MIN && ytl <= INT_MAX);
    SDL_assert(xbr >= INT_MIN && xbr <= INT_MAX);
    SDL_assert(ybr >= INT_MIN && ybr <= INT_MAX);

    return { xtl, ytl, xbr - xtl, ybr - ytl };
}