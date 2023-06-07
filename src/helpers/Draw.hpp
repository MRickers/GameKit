#pragma once
#include <cstddef>
#include <cstdint>
class SDL_Renderer;

namespace gk
{

struct ColorRGBA {
    uint8_t r{0};
    uint8_t g{0};
    uint8_t b{0};
    uint8_t a{0};
};

enum class Color {
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


class Draw
{
public:
    static void
    setRendererColor(SDL_Renderer* renderer, Color);
    static ColorRGBA
    getColorRGBA(Color color);
    static void
    filledCircle(SDL_Renderer* renderer, int centerX, int centerY, size_t radius);
};
} // namespace gk