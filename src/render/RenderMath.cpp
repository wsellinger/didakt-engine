#include "RenderMath.h"

#include <SDL_assert.h>

#include <climits>
#include <cmath>

namespace RenderMath
{
    SDL_Rect GetTileSourceRect(int id, int width, int height, int columns)
    {
        SDL_assert(id >= 0);
        SDL_assert(columns > 0);
        SDL_assert(width > 0);
        SDL_assert(height > 0);

        return
        {
            (id % columns) * width,
            (id / columns) * height,
            width,
            height
        };
    }

    SDL_Rect GetTileDestinationRect(size_t iRow, size_t iCol, int width, int height, const Camera& camera)
    {
        SDL_assert(iCol < INT_MAX);
        SDL_assert(iRow < INT_MAX);
        SDL_assert(width > 0);
        SDL_assert(height > 0);
        SDL_assert(camera.zoom > 0);

        float fCol = static_cast<float>(iCol);
        float fRow = static_cast<float>(iRow);
        float fWidth = static_cast<float>(width);
        float fHeight = static_cast<float>(height);

        glm::vec2 topLeft = camera.WorldToScreen({ fCol * fWidth, fRow * fHeight });
        glm::vec2 bottomRight = camera.WorldToScreen({ (fCol + 1) * fWidth, (fRow + 1) * fHeight });

        int xtl = static_cast<int>(std::round(topLeft.x));
        int ytl = static_cast<int>(std::round(topLeft.y));
        int xbr = static_cast<int>(std::round(bottomRight.x));
        int ybr = static_cast<int>(std::round(bottomRight.y));

        SDL_assert(xtl >= INT_MIN && xtl <= INT_MAX);
        SDL_assert(ytl >= INT_MIN && ytl <= INT_MAX);
        SDL_assert(xbr >= INT_MIN && xbr <= INT_MAX);
        SDL_assert(ybr >= INT_MIN && ybr <= INT_MAX);

        return { xtl, ytl, xbr - xtl, ybr - ytl };
    }

    SDL_Rect GetSpriteDestinationRect(glm::vec2 worldPosition, glm::vec2 worldSize, const Camera& camera)
    {
        glm::vec2 topLeft = camera.WorldToScreen(worldPosition);
        glm::vec2 bottomRight = camera.WorldToScreen(worldPosition + worldSize);

        int xtl = static_cast<int>(std::round(topLeft.x));
        int ytl = static_cast<int>(std::round(topLeft.y));
        int xbr = static_cast<int>(std::round(bottomRight.x));
        int ybr = static_cast<int>(std::round(bottomRight.y));

        return { xtl, ytl, xbr - xtl, ybr - ytl };
    }
}