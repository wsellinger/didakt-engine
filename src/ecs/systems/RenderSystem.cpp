#include "RenderSystem.h"

#include "../../render/AssetManager.h"
#include "../../render/Camera.h"
#include "../../render/RenderMath.h"
#include "../components/RenderComponent.h"
#include "../components/SpriteSheetComponent.h"
#include "../components/TilemapComponent.h"
#include "../components/TransformComponent.h"

#include <algorithm>
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
    TextureHandle handle = assetManager.GetTextureHandle(render.textureId);
    SDL_assert(handle.IsValid());
    SDL_Texture* texture = static_cast<SDL_Texture*>(handle.handle);

    //Source
    SDL_Rect source{ sprite.x, sprite.y, sprite.width, sprite.height };

    //Destination
    glm::vec2 worldSize{ sprite.width * transform.scale.x, sprite.height * transform.scale.y };
    SDL_Rect destination = RenderMath::GetSpriteDestinationRect(transform.position, worldSize, camera);

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
    TextureHandle handle = assetManager.GetTextureHandle(render.textureId);
    SDL_assert(handle.IsValid());
    SDL_Texture* texture = static_cast<SDL_Texture*>(handle.handle);

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
            SDL_Rect source = RenderMath::GetTileSourceRect(id, tilemap.tileWidth, tilemap.tileHeight, tilemap.sheetColumns);
            SDL_Rect destination = RenderMath::GetTileDestinationRect(iRow, iCol, tilemap.tileWidth, tilemap.tileHeight, camera);

            SDL_RenderCopy(renderer, texture, &source, &destination);
        }
    }
}