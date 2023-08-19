#pragma once
#include <cstddef>
#include <functional>
#include <vector>

#include "vector/Vector2d.hpp"

class SDL_Renderer;
namespace game
{
  class Snake
  {
    using SnakeContainer = std::vector<gk::Vector2D>;

  public:
    enum class Direction
    {
      up,
      down,
      left,
      right
    };

    using ScoreCallback = std::function<void(const size_t score)>;

    Snake(const size_t blocksize = 16);

    void update();
    void draw(SDL_Renderer*);

    void reset(const gk::Vector2D& startPos);
    void setDirection(const Snake::Direction);
    Direction getDirection() const;
    size_t getSpeed() const;
    gk::Vector2D getPosition() const;

    size_t getLives() const;
    void setLives(const size_t lives);

    void extend();

  private:
    size_t m_size;
    gk::Vector2D m_bodyRect;
    SnakeContainer m_snakeSegments;
    Direction m_direction{Direction::right};
    size_t m_speed;
    size_t m_lives;
  };
} // namespace game
