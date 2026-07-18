#include "SDLAssetProvider.h"

#include "../../core/Logger.h"

#include <SDL_error.h>
#include <SDL_image.h>
#include <SDL_surface.h>

TextureHandle SDLAssetProvider::LoadTexture(const std::string& path)
{
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface)
    {
        Logger::Log(LogLevel::Error, "Failed to load texture: %s", IMG_GetError());
        return TextureHandle{};
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture)
    {
        Logger::Log(LogLevel::Error, "Failed to create texture: %s", SDL_GetError());
        return TextureHandle{};
    }

    return TextureHandle{ texture };
}

void SDLAssetProvider::DestroyTexture(TextureHandle texture)
{
    if (!texture.IsValid())
    {
        Logger::Log(LogLevel::Warning, "Attempted to destroy an invalid texture handle.");
        return;
    }
    
    SDL_DestroyTexture(static_cast<SDL_Texture*>(texture.handle));    
}