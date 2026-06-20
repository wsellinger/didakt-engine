#pragma once

#include <string>
#include <vector>

#include <SDL_rect.h>
#include <SDL_render.h>

#include "Camera.h"

using TileRow = std::vector<int>;
using TileGrid = std::vector<TileRow>;

struct Tilemap
{
    std::string textureId;
    TileGrid tiles;
    int tileWidth = 0;
    int tileHeight = 0;
    int sheetColumns = 0;
};

void RenderTilemap(const Tilemap& tilemap, SDL_Renderer* renderer, SDL_Texture* texture, const Camera& camera);

static SDL_Rect GetSourceRect(int id, const Tilemap& tilemap);
static SDL_Rect GetDestinationRect(size_t iRow, size_t iCol, const Tilemap& tilemap, const Camera& camera);