#include "GameKit/helpers/GameException.hpp"
#include <catch2/catch_test_macros.hpp>

#include <iostream>
void throws()
{
  throw gk::GameException("Exception occured", 1234);
}

TEST_CASE("GameException", "[exception]")
{
  REQUIRE_THROWS_AS(throws(), gk::GameException);
  try
  {
    throws();
  }
  catch (const std::exception& e)
  {
    std::cout << e.what() << std::endl;
  }
}