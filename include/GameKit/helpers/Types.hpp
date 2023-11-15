#pragma once
#include "GameKit/vector/Vector2d.hpp"
#include <cstdint>

namespace gk
{
  using time_ms = uint32_t;

  struct Shape
  {
    gk::Vector2D m_pos{0, 0};
    gk::Vector2D m_size{0, 0};
  };

} // namespace gk
