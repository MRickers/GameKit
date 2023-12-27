#include "GameKit/helpers/timer.hpp"
#include <catch2/catch_test_macros.hpp>
#include <thread>

TEST_CASE("Timer", "[timing]")
{
  gk::Timer timer;
  REQUIRE(!timer.is_started());

  timer.start();

  while (!timer.has_passed(1000))
  {
  }
  timer.stop();
  REQUIRE(timer.time_passed() >= 1000);
}