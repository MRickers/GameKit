#include "GameKit/core/ui/TextBox.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Add content", "[textboxCircularBuffer]")
{
  gk::TextCirculareBuffer buffer{4};

  SECTION("Simple push")
  {
    buffer.push("test");
    REQUIRE(buffer.get()[0] == "test");
  }
  SECTION("Multi push")
  {
    buffer.push("hello");
    buffer.push("there");
    buffer.push("general");
    buffer.push("kenobi");

    const auto content = buffer.get();
    REQUIRE(content[0] == "hello");
    REQUIRE(content[1] == "there");
    REQUIRE(content[2] == "general");
    REQUIRE(content[3] == "kenobi");
  }
  SECTION("Roll over")
  {
    buffer.push("hello");
    buffer.push("there");
    buffer.push("general");
    buffer.push("kenobi");
    buffer.push("hi");

    const auto content = buffer.get();
    REQUIRE(content[0] == "there");
    REQUIRE(content[1] == "general");
    REQUIRE(content[2] == "kenobi");
    REQUIRE(content[3] == "hi");
    REQUIRE(buffer.size() == 4);
  }
}

TEST_CASE("Remove", "[textboxCircularBuffer]")
{
  gk::TextCirculareBuffer buffer{4};
  SECTION("Push Pop simple")
  {
    buffer.push("test");
    buffer.push("123");
    buffer.pop();

    REQUIRE(buffer.get()[0] == "123");
    REQUIRE(buffer.size() == 1);
    buffer.pop();
    REQUIRE(buffer.size() == 0);
    buffer.pop();
  }
  SECTION("Clear")
  {
    buffer.push("test");
    buffer.push("123");
    buffer.push("test2");
    buffer.push("1234");

    buffer.clear();

    REQUIRE(buffer.size() == 0);
  }
}