#include "SDLRenderProvider.h"

#include "../../core/Assert.h"

namespace
{
    SDL_Rect ToSDLRect(const RenderRect& rect)
    {
        return { rect.x, rect.y, rect.w, rect.h };
    }

    SDL_Color ToSDLColor(RenderColor color)
    {
        const int Uint8_MAX = 255;
        DIDAKT_ASSERT(color.r >= 0 && color.r <= Uint8_MAX);
        DIDAKT_ASSERT(color.g >= 0 && color.g <= Uint8_MAX);
        DIDAKT_ASSERT(color.b >= 0 && color.b <= Uint8_MAX);
        DIDAKT_ASSERT(color.a >= 0 && color.a <= Uint8_MAX);

        return SDL_Color
        { 
            static_cast<Uint8>(color.r), 
            static_cast<Uint8>(color.g), 
            static_cast<Uint8>(color.b), 
            static_cast<Uint8>(color.a)
        };
    }
}

void SDLRenderProvider::Clear(RenderColor color)
{
    DIDAKT_ASSERT(_renderer != nullptr);

    SDL_Color sdlColor = ToSDLColor(color);
    SDL_SetRenderDrawColor(_renderer, sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a);
    SDL_RenderClear(_renderer);
}

void SDLRenderProvider::Present()
{
    DIDAKT_ASSERT(_renderer != nullptr);
    SDL_RenderPresent(_renderer);
}

void SDLRenderProvider::DrawTexture(TextureHandle texture, const RenderRect& source, const RenderRect& destination, double rotation)
{
    DIDAKT_ASSERT(_renderer != nullptr);
    DIDAKT_ASSERT(texture.IsValid());

    SDL_Texture* sdlTexture = static_cast<SDL_Texture*>(texture.handle);
    DIDAKT_ASSERT(sdlTexture != nullptr);

    SDL_Rect sdlSource = ToSDLRect(source);
    SDL_Rect sdlDestination = ToSDLRect(destination);

    SDL_RenderCopyEx(_renderer, sdlTexture, &sdlSource, &sdlDestination, rotation, nullptr, SDL_FLIP_NONE);
}
