#pragma once
#include "GameKit/vector/Vector2d.hpp"
#include <cstddef>
#include <cstdint>

class SDL_Renderer;

namespace gk
{

  struct ColorRGBA
  {
    uint8_t r{0};
    uint8_t g{0};
    uint8_t b{0};
    uint8_t a{0};
  };

  enum class Color
  {
    RED,
    GREEN,
    BLUE,
    YELLOW,
    ORANGE,
    PURPLE,
    PINK,
    CYAN,
    MAGENTA,
    LIME,
    TEAL,
    LAVENDER,
    BROWN,
    SILVER,
    GOLD,
    NAVY,
    INDIGO,
    MAROON,
    OLIVE,
    CORAL,
    TURQUOISE,
    PLUM,
    SLATE,
    CRIMSON,
    STEEL,
    BLACK,
    WHITE,
    GRAY,
    AQUA,
    VIOLET
  };

  class Drawer
  {
  public:
    static void set_render_color(SDL_Renderer* renderer, Color);
    static ColorRGBA get_color_rgba(Color color);
    static void draw_filled_circle(SDL_Renderer* renderer, int centerX,
                                   int centerY, size_t radius);
    static void draw_filled_rect(SDL_Renderer* renderer,
                                 const gk::Vector2d& pos,
                                 const gk::Vector2d& size);
  };
} // namespace gk