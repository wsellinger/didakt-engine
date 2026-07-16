#include "SDLAssetProvider.h"

#include <SDL_error.h>
#include <SDL_image.h>
#include <SDL_log.h>
#include <SDL_surface.h>

TextureHandle SDLAssetProvider::LoadTexture(const std::string& path)
{
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s", IMG_GetError());
        return TextureHandle{};
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create texture: %s", SDL_GetError());
        return TextureHandle{};
    }

    return TextureHandle{ texture };
}

void SDLAssetProvider::DestroyTexture(TextureHandle texture)
{
    if (!texture.IsValid())
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Attempted to destroy an invalid texture handle.");
        return;
    }
    
    SDL_DestroyTexture(static_cast<SDL_Texture*>(texture.handle));    
}