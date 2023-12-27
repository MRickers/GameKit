#include "GameKit/helpers/game_exception.hpp"
#include <catch2/catch_test_macros.hpp>

#include <iostream>
void throws()
{
  throw gk::game_exception("Exception occured", 1234);
}

TEST_CASE("game_exception", "[exception]")
{
  REQUIRE_THROWS_AS(throws(), gk::game_exception);
  try
  {
    throws();
  }
  catch (const std::exception& e)
  {
    std::cout << e.what() << std::endl;
  }
}