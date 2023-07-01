#pragma once
#include <SDL2/SDL.h>
namespace game
{
  class Snake
  {
    enum class Direction
    {
      up,
      down,
      left,
      right
    };

  public:
    Snake(const size_t body_size = 20);

    void handleEvent(SDL_Event*);

  private:
    size_t m_body_size{20};
    int m_x{0};
    int m_y{0};
    Direction m_direction{Direction::up};
  };
} // namespace game
