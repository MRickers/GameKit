#include "Vector2d.hpp"
#include <cmath>
#include <iostream>

gk::Vector2D::Vector2D(const float x, const float y)
    : m_x{x}
    , m_y{y}
{
}

gk::Vector2D gk::Vector2D::operator+(const Vector2D& other) const
{
  return {m_x + other.m_x, m_y + other.m_y};
}

gk::Vector2D& gk::Vector2D::operator+=(const Vector2D& other)
{
  m_x += other.m_x;
  m_y += other.m_y;
  return *this;
}

gk::Vector2D gk::Vector2D::operator-(const Vector2D& other) const
{
  return {m_x - other.m_x, m_y - other.m_y};
}

gk::Vector2D& gk::Vector2D::operator-=(const Vector2D& other)
{
  m_x -= other.m_x;
  m_y -= other.m_y;
  return *this;
}

gk::Vector2D gk::Vector2D::operator*(const float scalar) const
{
  return {m_x * scalar, m_y * scalar};
}

gk::Vector2D& gk::Vector2D::operator*=(const float scalar)
{
  m_x *= scalar;
  m_y *= scalar;
  return *this;
}

gk::Vector2D gk::Vector2D::operator/(const float scalar) const
{
  if (scalar == 0)
  {
    throw std::invalid_argument{"Divide by zero"};
  }
  return {m_x / scalar, m_y / scalar};
}

gk::Vector2D& gk::Vector2D::operator/=(const float scalar)
{
  if (scalar == 0)
  {
    throw std::invalid_argument{"Divide by zero error"};
  }
  m_x /= scalar;
  m_y /= scalar;
  return *this;
}

float gk::Vector2D::Length() const
{
  return std::sqrt(m_x * m_x + m_y * m_y);
}

void gk::Vector2D::SetLength(const float len)
{
  if (const auto length = Length(); len > 0)
  {
    const auto multiplier = len / length;

    m_x *= multiplier;
    m_y *= multiplier;
  }
}

void gk::Vector2D::Normalize()
{
  if (const auto len = Length(); len > 0)
  {
    *this *= 1 / Length();
  }
}

void gk::Vector2D::SetLimit(const float limit)
{
  if (Length() > limit)
  {
    SetLength(limit);
  }
}

std::pair<float, float> gk::Vector2D::Get() const
{
  return {m_x, m_y};
}

float gk::Vector2D::GetX() const
{
  return m_x;
}

float gk::Vector2D::GetY() const
{
  return m_y;
}
