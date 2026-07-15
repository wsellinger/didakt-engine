#pragma once

#include "Camera.h"

#include <SDL_rect.h>

namespace RenderMath
{
    SDL_Rect GetTileSourceRect(int id, int width, int height, int columns);
    SDL_Rect GetTileDestinationRect(size_t iRow, size_t iCol, int width, int height, const Camera& camera);
    SDL_Rect GetSpriteDestinationRect(glm::vec2 worldPosition, glm::vec2 worldSize, const Camera& camera);
}