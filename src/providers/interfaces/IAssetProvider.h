#pragma once

#include "../../render/TextureHandle.h"

#include <string>

class IAssetProvider
{
public:
    virtual ~IAssetProvider() = default;

    virtual TextureHandle LoadTexture(const std::string& path) = 0;
    virtual void DestroyTexture(TextureHandle texture) = 0;
};
