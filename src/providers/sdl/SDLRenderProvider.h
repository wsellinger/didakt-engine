#pragma once

#include "../interfaces/IRenderProvider.h"

#include <SDL_render.h>

class SDLRenderProvider : public IRenderProvider
{
public:
    void Initialize(SDL_Renderer* renderer) { _renderer = renderer; }

    void Clear(RenderColor color) override;
    void Present() override;
    void DrawTexture(TextureHandle texture, const RenderRect& source, const RenderRect& destination, double rotation) override;

private:
    SDL_Renderer* _renderer = nullptr;
};