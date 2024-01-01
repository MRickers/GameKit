#include "GameKit/vector/vector2d.hpp"
#include <cmath>
#include <stdexcept>

gk::vector2d::vector2d(const float x, const float y)
    : m_x{x}
    , m_y{y}
{
}

gk::vector2d::vector2d(const int x, const int y)
    : m_x{static_cast<float>(x)}
    , m_y{static_cast<float>(y)}
{
}
gk::vector2d gk::vector2d::operator+(const vector2d& other) const
{
  return {m_x + other.m_x, m_y + other.m_y};
}

gk::vector2d& gk::vector2d::operator+=(const vector2d& other)
{
  m_x += other.m_x;
  m_y += other.m_y;
  return *this;
}

gk::vector2d gk::vector2d::operator-(const vector2d& other) const
{
  return {m_x - other.m_x, m_y - other.m_y};
}

gk::vector2d& gk::vector2d::operator-=(const vector2d& other)
{
  m_x -= other.m_x;
  m_y -= other.m_y;
  return *this;
}

gk::vector2d gk::vector2d::operator*(const float scalar) const
{
  return {m_x * scalar, m_y * scalar};
}

gk::vector2d& gk::vector2d::operator*=(const float scalar)
{
  m_x *= scalar;
  m_y *= scalar;
  return *this;
}

gk::vector2d gk::vector2d::operator/(const float scalar) const
{
  if (scalar == 0)
  {
    throw std::invalid_argument{"Divide by zero"};
  }
  return {m_x / scalar, m_y / scalar};
}

gk::vector2d& gk::vector2d::operator/=(const float scalar)
{
  if (scalar == 0)
  {
    throw std::invalid_argument{"Divide by zero error"};
  }
  m_x /= scalar;
  m_y /= scalar;
  return *this;
}

float gk::vector2d::Length() const
{
  return std::sqrt(m_x * m_x + m_y * m_y);
}

void gk::vector2d::SetLength(const float len)
{
  if (const auto length = Length(); len > 0)
  {
    const auto multiplier = len / length;

    m_x *= multiplier;
    m_y *= multiplier;
  }
}

void gk::vector2d::Normalize()
{
  if (const auto len = Length(); len > 0)
  {
    *this *= 1 / Length();
  }
}

void gk::vector2d::SetLimit(const float limit)
{
  if (Length() > limit)
  {
    SetLength(limit);
  }
}

std::pair<float, float> gk::vector2d::Get() const
{
  return {m_x, m_y};
}

template <> float gk::vector2d::GetX<float>() const
{
  return m_x;
}

template <> int gk::vector2d::GetX<int>() const
{
  return static_cast<int>(m_x);
}

template <> uint16_t gk::vector2d::GetX<uint16_t>() const
{
  return static_cast<uint16_t>(m_x);
}

template <> float gk::vector2d::GetY<float>() const
{
  return m_y;
}

template <> int gk::vector2d::GetY<int>() const
{
  return static_cast<int>(m_y);
}

template <> uint16_t gk::vector2d::GetY<uint16_t>() const
{
  return static_cast<uint16_t>(m_y);
}