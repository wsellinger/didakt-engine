#include "RenderMath.h"

#include "../core/Assert.h"

#include <climits>
#include <cmath>

namespace RenderMath
{
    RenderRect GetTileSourceRect(int id, int width, int height, int columns)
    {
        DIDAKT_ASSERT(id >= 0);
        DIDAKT_ASSERT(columns > 0);
        DIDAKT_ASSERT(width > 0);
        DIDAKT_ASSERT(height > 0);

        return
        {
            (id % columns) * width,
            (id / columns) * height,
            width,
            height
        };
    }

    RenderRect GetTileDestinationRect(size_t iRow, size_t iCol, int width, int height, const Camera& camera)
    {
        DIDAKT_ASSERT(iCol < INT_MAX);
        DIDAKT_ASSERT(iRow < INT_MAX);
        DIDAKT_ASSERT(width > 0);
        DIDAKT_ASSERT(height > 0);
        DIDAKT_ASSERT(camera.zoom > 0);

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

        DIDAKT_ASSERT(xtl >= INT_MIN && xtl <= INT_MAX);
        DIDAKT_ASSERT(ytl >= INT_MIN && ytl <= INT_MAX);
        DIDAKT_ASSERT(xbr >= INT_MIN && xbr <= INT_MAX);
        DIDAKT_ASSERT(ybr >= INT_MIN && ybr <= INT_MAX);

        return { xtl, ytl, xbr - xtl, ybr - ytl };
    }

    RenderRect GetSpriteDestinationRect(glm::vec2 worldPosition, glm::vec2 worldSize, const Camera& camera)
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