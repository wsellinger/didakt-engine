#include "Tilemap.h"

//#include <vector>
#include <climits>

#include <SDL_render.h>
#include <SDL_rect.h>
#include <SDL_assert.h>

#include "Camera.h"

void RenderTilemap(const Tilemap& tilemap, SDL_Renderer* renderer, SDL_Texture* texture, const Camera& camera)
{
	const TileGrid& tiles = tilemap.tiles;
	size_t numRows = tiles.size();

	for (size_t iRow = 0; iRow < numRows; iRow++)
	{
		const TileRow& row = tiles[iRow];
		size_t numCol = row.size();

		for (size_t iCol = 0; iCol < numCol; iCol++)
		{
			int id = row[iCol];
			SDL_Rect source = GetSourceRect(id, tilemap);
			SDL_Rect destination = GetDestinationRect(iRow, iCol, tilemap, camera);

			SDL_RenderCopy(renderer, texture, &source, &destination);
		}
	}
}

static SDL_Rect GetSourceRect(int id, const Tilemap& tilemap)
{
    SDL_assert(id >= 0);
    SDL_assert(tilemap.sheetColumns > 0);

	return
    {
        (id % tilemap.sheetColumns) * tilemap.tileWidth,
        (id / tilemap.sheetColumns) * tilemap.tileHeight,
        tilemap.tileWidth,
        tilemap.tileHeight
    };
}

static SDL_Rect GetDestinationRect(size_t iRow, size_t iCol, const Tilemap& tilemap, const Camera& camera)
{
    SDL_assert(iCol < INT_MAX);
    SDL_assert(iRow < INT_MAX);
    SDL_assert(tilemap.tileWidth > 0);
    SDL_assert(tilemap.tileHeight > 0);
    SDL_assert(camera.zoom > 0);

    float fCol = static_cast<float>(iCol);
    float fRow = static_cast<float>(iRow);
    float fWidth = static_cast<float>(tilemap.tileWidth);
    float fHeight = static_cast<float>(tilemap.tileHeight);

    float x = (fCol * fWidth - camera.x) * camera.zoom;
    float y = (fRow * fHeight - camera.y) * camera.zoom;
    float w = fWidth * camera.zoom;
    float h = fHeight * camera.zoom;

    SDL_assert(x >= INT_MIN && x <= INT_MAX);
    SDL_assert(y >= INT_MIN && y <= INT_MAX);
    SDL_assert(w >= INT_MIN && w <= INT_MAX);
    SDL_assert(h >= INT_MIN && h <= INT_MAX);

	return { static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h) };
}