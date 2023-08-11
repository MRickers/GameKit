#include "helpers/Draw.hpp"

#include "Draw.hpp"
#include <SDL2/SDL_render.h>
namespace gk
{

  void Draw::setRendererColor(SDL_Renderer* renderer, Color color)
  {
    const auto rgba = getColorRGBA(color);
    SDL_SetRenderDrawColor(renderer, rgba.r, rgba.g, rgba.b, rgba.a);
  }

  ColorRGBA Draw::getColorRGBA(Color color)
  {
    switch (color)
    {
    case Color::RED:
      return {255, 0, 0, 255};
    case Color::GREEN:
      return {0, 255, 0, 255};
    case Color::BLUE:
      return {0, 0, 255, 255};
    case Color::YELLOW:
      return {255, 255, 0, 255};
    case Color::ORANGE:
      return {255, 165, 0, 255};
    case Color::PURPLE:
      return {128, 0, 128, 255};
    case Color::PINK:
      return {255, 192, 203, 255};
    case Color::CYAN:
      return {0, 255, 255, 255};
    case Color::MAGENTA:
      return {255, 0, 255, 255};
    case Color::LIME:
      return {0, 255, 0, 255};
    case Color::TEAL:
      return {0, 128, 128, 255};
    case Color::LAVENDER:
      return {230, 230, 250, 255};
    case Color::BROWN:
      return {165, 42, 42, 255};
    case Color::SILVER:
      return {192, 192, 192, 255};
    case Color::GOLD:
      return {255, 215, 0, 255};
    case Color::NAVY:
      return {0, 0, 128, 255};
    case Color::INDIGO:
      return {75, 0, 130, 255};
    case Color::MAROON:
      return {128, 0, 0, 255};
    case Color::OLIVE:
      return {128, 128, 0, 255};
    case Color::CORAL:
      return {255, 127, 80, 255};
    case Color::TURQUOISE:
      return {64, 224, 208, 255};
    case Color::PLUM:
      return {221, 160, 221, 255};
    case Color::SLATE:
      return {112, 128, 144, 255};
    case Color::CRIMSON:
      return {220, 20, 60, 255};
    case Color::STEEL:
      return {70, 130, 180, 255};
    case Color::BLACK:
      return {0, 0, 0, 255};
    case Color::WHITE:
      return {255, 255, 255, 255};
    case Color::GRAY:
      return {128, 128, 128, 255};
    case Color::AQUA:
      return {0, 255, 255, 255};
    case Color::VIOLET:
      return {238, 130, 238, 255};
    }

    // Return default color (black) if the given color is not recognized
    return {0, 0, 0, 255};
  }
  void Draw::filledCircle(SDL_Renderer* renderer, int centerX, int centerY,
                          size_t radius)
  {
    const int squaredRadius = radius * radius;
    const int minX = -radius;
    const int minY = -radius;
    const int maxX = radius;
    const int maxY = radius;

    for (int y = minY; y <= maxY; ++y)
    {
      for (int x = minX; x <= maxX; ++x)
      {
        const auto distanceSquared = x * x + y * y;
        if (distanceSquared <= squaredRadius)
        {
          SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
        }
      }
    }
  }
} // namespace gk

void gk::Draw::filledRect(SDL_Renderer* renderer, const gk::Vector2D& pos,
                          const gk::Vector2D& size)
{
  SDL_Rect rect{static_cast<int>(pos.GetX()), static_cast<int>(pos.GetY()),
                static_cast<int>(size.GetX()), static_cast<int>(size.GetY())};
  SDL_RenderFillRect(renderer, &rect);
}
