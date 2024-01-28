#include "GameKit/core/input_handler.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Add/Remove Callback", "[inputhandler]")
{
  gk::input_handler input_handler;
  SECTION("add_callback")
  {
    REQUIRE(
        input_handler.add_callback("test", [](const gk::event_details&) {}));
  }
  SECTION("remove_callback")
  {
    REQUIRE(
        input_handler.add_callback("test", [](const gk::event_details&) {}));
    REQUIRE(input_handler.remove_callback("test"));
  }
  SECTION("RemoveInvalidCallback")
  {
    REQUIRE(!input_handler.remove_callback("test"));
  }
}

TEST_CASE("Add/Remove Binding", "[inputhandler]")
{
  gk::input_handler input_handler;
  SECTION("add_binding")
  {
    REQUIRE(input_handler.add_keydown_binding("test", {}));
  }
  SECTION("remove_binding")
  {
    REQUIRE(input_handler.add_keydown_binding("test", {}));
    REQUIRE(input_handler.remove_binding("test"));
  }
  SECTION("RemoveInvalidBinding")
  {
    REQUIRE(!input_handler.remove_binding("test"));
  }
}