#include "vector/Vector2d.hpp"
#include <catch2/catch_test_macros.hpp>
using namespace gk;

TEST_CASE("Addition", "[vector2d]")
{
  Vector2D vec1{1, 1};
  Vector2D vec2{2, 2};

  const auto vec3 = vec1 + vec2;

  const auto [x, y] = vec3.Get();

  REQUIRE(x == 3);
  REQUIRE(y == 3);
}

TEST_CASE("AdditionTo", "[vector2d]")
{
  Vector2D vec1{23, 1000};
  Vector2D vec2{9876, 234};

  vec2 += vec1;

  const auto [x, y] = vec2.Get();

  REQUIRE(x == 9899);
  REQUIRE(y == 1234);
  REQUIRE(vec1.GetX() == 23);
  REQUIRE(vec1.GetY() == 1000);
}

TEST_CASE("Subtraction", "[vector2d]")
{
  Vector2D vec1{1, 1};
  Vector2D vec2{3, 3};

  const auto vec3 = vec1 - vec2;

  const auto [x, y] = vec3.Get();

  REQUIRE(x == -2);
  REQUIRE(y == -2);
}

TEST_CASE("SubstractTo", "[vector2d]")
{
  Vector2D vec1{300, 400};
  Vector2D vec2{500, 600};

  vec1 -= vec2;

  const auto [x, y] = vec1.Get();

  REQUIRE(x == -200);
  REQUIRE(y == -200);
  REQUIRE(vec2.GetX() == 500);
  REQUIRE(vec2.GetY() == 600);
}

TEST_CASE("MultiplyScalar", "[vector2d]")
{
  Vector2D vec1{300, 400};
  const auto vec2 = vec1 * 4;

  const auto [x, y] = vec2.Get();

  REQUIRE(x == 1200);
  REQUIRE(y == 1600);
  REQUIRE(vec1.GetX() == 300);
  REQUIRE(vec1.GetY() == 400);
}

TEST_CASE("MultiplyScalarTo", "[vector2d]")
{
  Vector2D vec1{300, 400};
  vec1 *= 10;

  const auto [x, y] = vec1.Get();

  REQUIRE(x == 3000);
  REQUIRE(y == 4000);
}

TEST_CASE("DivideScalar", "[vector2d]")
{
  Vector2D vec1{300, 400};
  const auto vec2 = vec1 / 2;

  const auto [x, y] = vec2.Get();

  REQUIRE(x == 150);
  REQUIRE(y == 200);
  REQUIRE(vec1.GetX() == 300);
  REQUIRE(vec1.GetY() == 400);
}

TEST_CASE("DivideScalarTo", "[vector2d]")
{
  Vector2D vec1{300, 400};
  vec1 /= 10;

  const auto [x, y] = vec1.Get();

  REQUIRE(x == 30);
  REQUIRE(y == 40);
}

TEST_CASE("DivideByZero", "[vector2d]")
{
  Vector2D vec1{300, 400};
  REQUIRE_THROWS_AS(vec1 / 0, std::invalid_argument);
}

TEST_CASE("DivideByZeroTo", "[vector2d]")
{
  Vector2D vec1{300, 400};
  REQUIRE_THROWS_AS(vec1 /= 0, std::invalid_argument);
}

TEST_CASE("VectorLength", "[vector2d]")
{
  Vector2D vec1{3, 4};
  REQUIRE(vec1.Length() == 5);
}

TEST_CASE("Normalize", "[vector2d]")
{
  Vector2D vec1{3, 4};
  REQUIRE_NOTHROW(vec1.Normalize());
  REQUIRE(vec1.Length() == 1);
}

TEST_CASE("SetLength", "[vector2d]")
{
  Vector2D vec1{3, 4};
  REQUIRE(vec1.Length() == 5);
  vec1.SetLength(10);
  REQUIRE(vec1.Length() == 10);
}

TEST_CASE("SetLimitNotReached", "[vector2d]")
{
  Vector2D vec1{3, 4};
  REQUIRE(vec1.Length() == 5);
  vec1.SetLimit(10);
  REQUIRE(vec1.Length() == 5);
}

TEST_CASE("SetLimitReached", "[vector2d]")
{
  Vector2D vec1{3, 4};
  REQUIRE(vec1.Length() == 5);
  vec1.SetLimit(3);
  REQUIRE(vec1.Length() == 3);
}