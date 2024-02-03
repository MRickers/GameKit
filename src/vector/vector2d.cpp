#include "GameKit/vector/Vector2d.hpp"
#include <cmath>
#include <stdexcept>

gk::Vector2d::Vector2d(const float x, const float y)
    : m_x{x}
    , m_y{y}
{
}

gk::Vector2d::Vector2d(const int x, const int y)
    : m_x{static_cast<float>(x)}
    , m_y{static_cast<float>(y)}
{
}
gk::Vector2d gk::Vector2d::operator+(const Vector2d& other) const
{
  return {m_x + other.m_x, m_y + other.m_y};
}

gk::Vector2d& gk::Vector2d::operator+=(const Vector2d& other)
{
  m_x += other.m_x;
  m_y += other.m_y;
  return *this;
}

gk::Vector2d gk::Vector2d::operator-(const Vector2d& other) const
{
  return {m_x - other.m_x, m_y - other.m_y};
}

gk::Vector2d& gk::Vector2d::operator-=(const Vector2d& other)
{
  m_x -= other.m_x;
  m_y -= other.m_y;
  return *this;
}

gk::Vector2d gk::Vector2d::operator*(const float scalar) const
{
  return {m_x * scalar, m_y * scalar};
}

gk::Vector2d& gk::Vector2d::operator*=(const float scalar)
{
  m_x *= scalar;
  m_y *= scalar;
  return *this;
}

gk::Vector2d gk::Vector2d::operator/(const float scalar) const
{
  if (scalar == 0)
  {
    throw std::invalid_argument{"Divide by zero"};
  }
  return {m_x / scalar, m_y / scalar};
}

gk::Vector2d& gk::Vector2d::operator/=(const float scalar)
{
  if (scalar == 0)
  {
    throw std::invalid_argument{"Divide by zero error"};
  }
  m_x /= scalar;
  m_y /= scalar;
  return *this;
}

float gk::Vector2d::Length() const
{
  return std::sqrt(m_x * m_x + m_y * m_y);
}

void gk::Vector2d::SetLength(const float len)
{
  if (const auto length = Length(); len > 0)
  {
    const auto multiplier = len / length;

    m_x *= multiplier;
    m_y *= multiplier;
  }
}

void gk::Vector2d::Normalize()
{
  if (const auto len = Length(); len > 0)
  {
    *this *= 1 / Length();
  }
}

void gk::Vector2d::SetLimit(const float limit)
{
  if (Length() > limit)
  {
    SetLength(limit);
  }
}

std::pair<float, float> gk::Vector2d::Get() const
{
  return {m_x, m_y};
}

template <> float gk::Vector2d::GetX<float>() const
{
  return m_x;
}

template <> int gk::Vector2d::GetX<int>() const
{
  return static_cast<int>(m_x);
}

template <> uint16_t gk::Vector2d::GetX<uint16_t>() const
{
  return static_cast<uint16_t>(m_x);
}

template <> float gk::Vector2d::GetY<float>() const
{
  return m_y;
}

template <> int gk::Vector2d::GetY<int>() const
{
  return static_cast<int>(m_y);
}

template <> uint16_t gk::Vector2d::GetY<uint16_t>() const
{
  return static_cast<uint16_t>(m_y);
}