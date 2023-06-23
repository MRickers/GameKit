#pragma once

namespace game
{
  class Snake
  {
  public:
    Snake(const size_t body_size = 20);

  private:
    size_t m_body_size{20};
  };
} // namespace game
