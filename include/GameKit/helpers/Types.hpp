#pragma once
#include "GameKit/vector/Vector2d.hpp"
#include <cstdint>

namespace gk
{
  using time_ms = uint32_t;

  struct Rect
  {
    Rect() = default;
    Rect(gk::Vector2d const& t_pos, gk::Vector2d const& t_size)
        : pos{t_pos}
        , size{t_size}
    {
    }
    gk::Vector2d pos{0, 0};
    gk::Vector2d size{0, 0};
  };

} // namespace gk
