#include "GameKit/helpers/Timer.hpp"
#include <catch2/catch_test_macros.hpp>
#include <thread>

TEST_CASE("Timer", "[timing]")
{
  gk::Timer timer;
  REQUIRE(!timer.IsStarted());

  timer.Start();

  while (!timer.HasPassed(1000))
  {
  }
  timer.Stop();
  REQUIRE(timer.TimePassed() >= 1000);
}