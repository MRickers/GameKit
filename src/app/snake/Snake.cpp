#include "Snake.hpp"

game::Snake::Snake(const size_t blocksize)
    : m_size{blocksize}
    , m_bodyRect{static_cast<float>(m_size - 1), static_cast<float>(m_size - 1)}
    , m_speed{15}
{
  reset({5, 7});
}

void game::Snake::reset(const gk::Vector2D& startPos)
{
  m_snakeSegments.clear();

  m_snakeSegments.push_back(startPos);
  m_snakeSegments.push_back(startPos - gk::Vector2D{0, -1});
  m_snakeSegments.push_back(startPos - gk::Vector2D{0, -2});

  m_direction = Snake::Direction::right;
  m_speed = 15;
  m_lives = 3;
}

void game::Snake::setDirection(const Snake::Direction dir)
{
  m_direction = dir;
}

game::Snake::Direction game::Snake::getDirection() const
{
  return m_direction;
}

size_t game::Snake::getSpeed() const
{
  return m_speed;
}

gk::Vector2D game::Snake::getPosition() const
{
  return m_snakeSegments.front();
}

size_t game::Snake::getLives() const
{
  return m_lives;
}

void game::Snake::setLives(const size_t lives)
{
  m_lives = lives;
}

void game::Snake::extend()
{
  if (m_snakeSegments.empty())
  {
    return;
  }

  auto& tailHead = m_snakeSegments.back();

  if (m_snakeSegments.size() > 1)
  {
    auto& tailBone = m_snakeSegments.at(m_snakeSegments.size() - 2);

    const auto& [tailHeadX, tailHeadY] = tailHead.Get();
  }
  else
  {
  }
}
