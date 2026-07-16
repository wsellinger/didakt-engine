#pragma once

#include "Camera.h"
#include "RenderRect.h"

namespace RenderMath
{
    RenderRect GetTileSourceRect(int id, int width, int height, int columns);
    RenderRect GetTileDestinationRect(size_t iRow, size_t iCol, int width, int height, const Camera& camera);
    RenderRect GetSpriteDestinationRect(glm::vec2 worldPosition, glm::vec2 worldSize, const Camera& camera);
}