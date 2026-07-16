#pragma once

#include "../../render/RenderColor.h"
#include "../../render/RenderRect.h"
#include "../../render/TextureHandle.h"

class IRenderProvider
{
public:
    virtual ~IRenderProvider() = default;

    virtual void Clear(RenderColor) = 0;
    virtual void Present() = 0;

    virtual void DrawTexture(TextureHandle texture, const RenderRect& source, const RenderRect& destination, double rotation) = 0;
};