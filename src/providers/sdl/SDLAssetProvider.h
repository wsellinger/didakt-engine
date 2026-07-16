#pragma once

#include "../interfaces/IAssetProvider.h"

#include <SDL_render.h>

class SDLAssetProvider : public IAssetProvider
{
public:
    void Initialize(SDL_Renderer* renderer) { _renderer = renderer; }

    TextureHandle LoadTexture(const std::string& path) override;
    void DestroyTexture(TextureHandle texture) override;

private:
    SDL_Renderer* _renderer;
};