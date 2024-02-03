#pragma once
#include <limits>
#include <utility>
namespace gk
{
  class Vector2d
  {
  public:
    Vector2d(const float x, const float y);
    Vector2d(const int x, const int y);
    Vector2d() = default;
    Vector2d operator+(const Vector2d&) const;
    Vector2d& operator+=(const Vector2d&);
    Vector2d operator-(const Vector2d&) const;
    Vector2d& operator-=(const Vector2d&);
    Vector2d operator*(const float) const;
    Vector2d& operator*=(const float);
    Vector2d operator/(const float) const;
    Vector2d& operator/=(const float);

    float Length() const;
    void SetLength(const float len);
    void Normalize();
    /// @brief Sets Magnitude limit of vector.
    /// @param limit
    void SetLimit(const float limit);

    template <typename T> T GetX() const;
    template <typename T> T GetY() const;

    std::pair<float, float> Get() const;

  private:
    float m_x{0};
    float m_y{0};
    float m_limit{std::numeric_limits<float>::max()};
  };
} // namespace gk
