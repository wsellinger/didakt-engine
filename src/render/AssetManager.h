#pragma once

#include "../providers/interfaces/IAssetProvider.h"
#include "texturehandle.h"

#include <string>
#include <unordered_map>

class AssetManager
{
public:
	AssetManager() = default;
    ~AssetManager();

    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;
    AssetManager(AssetManager&&) = delete;
    AssetManager& operator=(AssetManager&&) = delete;

    void Initialize(IAssetProvider& provider) { _provider = &provider; }

	void LoadTexture(const std::string& id, const std::string& path);
	TextureHandle GetTextureHandle(const std::string& id) const;
	void ClearAll();

private:
    IAssetProvider* _provider = nullptr;

	std::unordered_map<std::string, TextureHandle> _handles;
};