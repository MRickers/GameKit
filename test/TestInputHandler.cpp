#include "core/InputHandler.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Add/Remove Callback", "[inputhandler]")
{
  gk::InputHandler input_handler;
  SECTION("AddCallback")
  {
    REQUIRE(input_handler.AddCallback("test", [](const gk::EventDetails&) {}));
  }
  SECTION("RemoveCallback")
  {
    REQUIRE(input_handler.AddCallback("test", [](const gk::EventDetails&) {}));
    REQUIRE(input_handler.RemoveCallback("test"));
  }
  SECTION("RemoveInvalidCallback")
  {
    REQUIRE(!input_handler.RemoveCallback("test"));
  }
}

TEST_CASE("Add/Remove Binding", "[inputhandler]")
{
  gk::InputHandler input_handler;
  SECTION("AddBinding")
  {
    REQUIRE(input_handler.AddBinding("test", {}));
  }
  SECTION("RemoveBinding")
  {
    REQUIRE(input_handler.AddBinding("test", {}));
    REQUIRE(input_handler.RemoveBinding("test"));
  }
  SECTION("RemoveInvalidBinding")
  {
    REQUIRE(!input_handler.RemoveBinding("test"));
  }
}