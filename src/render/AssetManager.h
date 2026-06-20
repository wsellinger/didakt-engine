#pragma once

#include <string>
#include <unordered_map>

#include <SDL_render.h>

class AssetManager
{
public:
	AssetManager() = default;
    ~AssetManager();

    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;
    AssetManager(AssetManager&&) = delete;
    AssetManager& operator=(AssetManager&&) = delete;

	void LoadTexture(const std::string& id, const std::string& filepath, SDL_Renderer* renderer);
	SDL_Texture* GetTexture(const std::string& id) const;
	void ClearAll();

private:
	std::unordered_map<std::string, SDL_Texture*> _textures;
};