#pragma once
#include <limits>
#include <utility>
namespace gk
{
  class vector2d
  {
  public:
    vector2d(const float x, const float y);
    vector2d(const int x, const int y);
    vector2d() = default;
    vector2d operator+(const vector2d&) const;
    vector2d& operator+=(const vector2d&);
    vector2d operator-(const vector2d&) const;
    vector2d& operator-=(const vector2d&);
    vector2d operator*(const float) const;
    vector2d& operator*=(const float);
    vector2d operator/(const float) const;
    vector2d& operator/=(const float);

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
