#include "AssetManager.h"

#include "../core/Logger.h"

using string = std::string;

AssetManager::~AssetManager()
{
    ClearAll();
}

void AssetManager::LoadTexture(const string& id, const string& path)
{
    //Check Loaded
    if (_handles.count(id))
        return;
    
    //Load Texture
    TextureHandle handle = _provider->LoadTexture(path);
    if (!handle.IsValid())
        return;

    //Store Handle
    _handles[id] = handle;
}

TextureHandle AssetManager::GetTextureHandle(const string& id) const
{
    auto it = _handles.find(id);
    if (it == _handles.end())
    {
        Logger::Log(LogLevel::Error, "AssetManager: texture not found - %s", id.c_str());
        return TextureHandle{};
    }

    return it->second;
}

void AssetManager::ClearAll()
{
    for (auto& [id, handle] : _handles)
        _provider->DestroyTexture(handle);

    _handles.clear();
}
