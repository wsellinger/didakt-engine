#include "RenderSystem.h"

#include "../../render/AssetManager.h"
#include "../../render/Camera.h"
#include "../components/SpriteComponent.h"
#include "../components/TransformComponent.h"

#include <algorithm>
#include <vector>

#include <entt/entity/fwd.hpp>
#include <entt/entity/registry.hpp>

#include <SDL_assert.h>
#include <SDL_rect.h>
#include <SDL_render.h>

void RenderSystem::Render(entt::registry& registry, SDL_Renderer* renderer, AssetManager& assetManager, const Camera& camera)
{
    //Get and Sort View
    auto view = registry.view<TransformComponent, SpriteComponent>();
    std::vector<entt::entity> entities(view.begin(), view.end());
    std::sort(entities.begin(), entities.end(),
        [&view](entt::entity a, entt::entity b)
        {
            return view.get<SpriteComponent>(a).z < view.get<SpriteComponent>(b).z; //Sort by Z Index Ascending
        });

    for (auto entity : entities)
    {
        //Get Components
        const auto& transform = view.get<TransformComponent>(entity);
        const auto& sprite = view.get<SpriteComponent>(entity);

        //Texture
        SDL_Texture* texture = assetManager.GetTexture(sprite.textureId);
        SDL_assert(texture != nullptr);

        //Source
        SDL_Rect source
        {
            sprite.x,
            sprite.y,
            sprite.width,
            sprite.height
        };

        //Destination
        SDL_Rect destination
        {
            static_cast<int>((transform.position.x - camera.x) * camera.zoom),
            static_cast<int>((transform.position.y - camera.y) * camera.zoom),
            static_cast<int>(sprite.width * transform.scale.x * camera.zoom),
            static_cast<int>(sprite.height * transform.scale.y * camera.zoom)
        };

        //Render
        SDL_RenderCopyEx(renderer, texture, &source, &destination, transform.rotation, nullptr, SDL_FLIP_NONE);
    }
}
