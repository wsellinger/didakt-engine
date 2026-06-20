#include "AssetManager.h"

#include <string>

#include <SDL_error.h>
#include <SDL_image.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_log.h>

using string = std::string;

AssetManager::~AssetManager()
{
    ClearAll();
}

void AssetManager::LoadTexture(const string& id, const string& filepath, SDL_Renderer* renderer)
{
    //Check if Loaded
    if (_textures.count(id))
        return;

    //Get Surface
    SDL_Surface* surface = IMG_Load(filepath.c_str());
    if (!surface)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "AssetManager: failed to load %s - %s", filepath.c_str(), IMG_GetError());
        return;
    }

    //Convert to Texture
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "AssetManager: failed to create texture for %s - %s", id.c_str(), SDL_GetError());
        return;
    }

    //Store Texture
    _textures[id] = texture;
}

SDL_Texture* AssetManager::GetTexture(const string& id) const
{
    auto it = _textures.find(id);
    if (it == _textures.end())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "AssetManager: texture not found - %s", id.c_str());
        return nullptr;
    }

    return it->second;
}

void AssetManager::ClearAll()
{
    for (auto& [id, texture] : _textures)
        SDL_DestroyTexture(texture);

    _textures.clear();
}
