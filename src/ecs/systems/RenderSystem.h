#pragma once

#include "../../render/Camera.h"

#include "vector"

#include <entt/entity/fwd.hpp>
#include <SDL_render.h>
#include <SDL_rect.h>

class AssetManager;
class TilemapComponent;

class RenderSystem
{
public:
    RenderSystem() = default;
    ~RenderSystem() = default;

    RenderSystem(const RenderSystem&) = delete;
    RenderSystem& operator=(const RenderSystem&) = delete;
    RenderSystem(RenderSystem&&) = delete;
    RenderSystem& operator=(RenderSystem&&) = delete;

    void Render(entt::registry& registry, SDL_Renderer* renderer, AssetManager& assetManager, const Camera& camera);

private:
    enum class DrawType { Invalid, Sprite, Tilemap };

    struct DrawCommand
    {
        entt::entity entity{};
        DrawType type{};
        int zIndex{};
    };

    struct RenderParameters
    {
        entt::registry& registry;
        SDL_Renderer* renderer{};
        AssetManager& assetManager;
        const Camera& camera;
    };

    template<typename... Components>
    static void AppendCommands(DrawType type, entt::registry& registry, std::vector<DrawCommand>& drawCommands);

    static void RenderSprite(entt::entity entity, const RenderParameters& renderParameters);
    static void RenderTilemap(entt::entity entity, const RenderParameters& renderParameters);
};
