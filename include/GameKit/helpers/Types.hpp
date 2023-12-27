#pragma once
#include "GameKit/vector/vector2d.hpp"
#include <cstdint>

namespace gk
{
  using time_ms = uint32_t;

  struct Shape
  {
    gk::vector2d m_pos{0, 0};
    gk::vector2d m_size{0, 0};
  };

} // namespace gk
