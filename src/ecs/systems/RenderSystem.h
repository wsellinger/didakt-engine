#pragma once

#include "../../render/Camera.h"
#include <SDL_render.h>
#include <entt/entity/fwd.hpp>

class AssetManager;

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
};
