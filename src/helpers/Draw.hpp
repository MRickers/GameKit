#pragma once
#include <cstddef>
class SDL_Renderer;

namespace helpers
{
class Draw
{
public:
    static void
    filledCircle(SDL_Renderer* renderer, int centerX, int centerY, size_t radius);
};
} // namespace helpers