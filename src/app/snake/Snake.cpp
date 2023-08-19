#include "Snake.hpp"

game::Snake::Snake(const size_t blocksize)
    : m_size{blocksize}
    , m_bodyRect{static_cast<float>(m_size - 1), static_cast<float>(m_size - 1)}
    , m_speed{15}
{
  reset({5, 7});
}

void game::Snake::update()
{
  if (m_snakeSegments.empty())
  {
    return;
  }
  move();
  if (const auto segmentIndex = checkCollision(); segmentIndex != 0)
  {
  }
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

  const auto& [tailHeadX, tailHeadY] = m_snakeSegments.back().Get();

  if (m_snakeSegments.size() > 1)
  {
    auto& tailBone = m_snakeSegments.at(m_snakeSegments.size() - 2);

    const auto& [tailBoneX, tailBoneY] = tailBone.Get();

    if (tailHeadX == tailBoneX)
    {
      if (tailHeadY > tailBoneY)
      {
        m_snakeSegments.push_back({tailHeadX, tailHeadY + 1});
      }
      else
      {
        m_snakeSegments.push_back({tailHeadX, tailHeadY - 1});
      }
    }
    else if (tailHeadY == tailBoneY)
    {
      if (tailHeadX > tailBoneX)
      {
        m_snakeSegments.push_back({tailHeadX + 1, tailHeadY});
      }
      else
      {
        m_snakeSegments.push_back({tailHeadX - 1, tailHeadY});
      }
    }
  }
  else
  {
    if (m_direction == Direction::up)
    {
      m_snakeSegments.push_back({tailHeadX, tailHeadY + 1});
    }
    else if (m_direction == Direction::down)
    {
      m_snakeSegments.push_back({tailHeadX, tailHeadY - 1});
    }
    else if (m_direction == Direction::left)
    {
      m_snakeSegments.push_back({tailHeadX + 1, tailHeadY});
    }
    else if (m_direction == Direction::right)
    {
      m_snakeSegments.push_back({tailHeadX - 1, tailHeadY});
    }
  }
}

void game::Snake::move()
{
  for (int i = m_snakeSegments.size() - 1; i > 0; --i)
  {
    m_snakeSegments[i] = m_snakeSegments[i - 1];
  }

  if (m_direction == Direction::left)
  {
    m_snakeSegments[0] -= {1, 0};
  }
  else if (m_direction == Direction::right)
  {
    m_snakeSegments[0] += {1, 0};
  }
  else if (m_direction == Direction::up)
  {
    m_snakeSegments[0] -= {0, 1};
  }
  else if (m_direction == Direction::down)
  {
    m_snakeSegments[0] += {0, 1};
  }
}

size_t game::Snake::checkCollision() const
{
  if (m_snakeSegments.size() > 4)
  {
    const auto& [headX, headY] = m_snakeSegments.front().Get();
    for (auto it = m_snakeSegments.begin() + 1; it != m_snakeSegments.end();
         ++it)
    {
      const auto& [x, y] = it->Get();
      if (static_cast<size_t>(x) == static_cast<size_t>(headX) &&
          static_cast<size_t>(y) == static_cast<size_t>(headY))
      {
        return m_snakeSegments.end() - it;
      }
    }
  }
  return 0;
}

void game::Snake::cut(const size_t segment)
{
  m_snakeSegments.erase(m_snakeSegments.begin() + segment,
                        m_snakeSegments.end());
  --m_lives;
}
