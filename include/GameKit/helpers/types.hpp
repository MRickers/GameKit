#pragma once
#include "GameKit/vector/vector2d.hpp"
#include <cstdint>

namespace gk
{
  using time_ms = uint32_t;

  struct rect
  {
    rect() = default;
    rect(gk::vector2d const& t_pos, gk::vector2d const& t_size)
        : pos{t_pos}
        , size{t_size}
    {
    }
    gk::vector2d pos{0, 0};
    gk::vector2d size{0, 0};
  };

} // namespace gk
