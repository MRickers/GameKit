#pragma once
#include <limits>
#include <utility>

namespace gk
{
  class Vector2D
  {
  public:
    Vector2D(const float x, const float y);
    Vector2D() = default;
    Vector2D operator+(const Vector2D&) const;
    Vector2D& operator+=(const Vector2D&);
    Vector2D operator-(const Vector2D&) const;
    Vector2D& operator-=(const Vector2D&);
    Vector2D operator*(const float) const;
    Vector2D& operator*=(const float);
    Vector2D operator/(const float) const;
    Vector2D& operator/=(const float);

    float Length() const;
    void SetLength(const float len);
    void Normalize();
    /// @brief Sets Magnitude limit of vector.
    /// @param limit
    void SetLimit(const float limit);

    std::pair<float, float> Get() const;
    float GetX() const;
    float GetY() const;

  private:
    float m_x{0};
    float m_y{0};
    float m_limit{std::numeric_limits<float>::max()};
  };

} // namespace gk
