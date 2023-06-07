#include "helpers/Draw.hpp"

#include <SDL2/SDL_render.h>
namespace helpers
{
void
Draw::filledCircle(SDL_Renderer* renderer, int centerX, int centerY, size_t radius)
{
    const int squaredRadius = radius * radius;
    const int minX = -radius;
    const int minY = -radius;
    const int maxX = radius;
    const int maxY = radius;

    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            const auto distanceSquared = x * x + y * y;
            if (distanceSquared <= squaredRadius) {
                SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
            }
        }
    }
}
} // namespace helpers