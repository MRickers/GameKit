#include "GameKit/core/texture_manager.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Interface", "[texture_manager]")
{
  SDL_Window* window =
      SDL_CreateWindow("Texturemanager test", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 475, 475, 0);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

  SECTION("Default construct")
  {
    gk::texture_manager texture_manager{renderer};
    SDL_Texture* t = texture_manager.load("test_texture.png");
    REQUIRE(t != nullptr);
  }

  SECTION("load invalid config constructor")
  {
    gk::texture_manager texture_manager{renderer, "test_texture.png"};
    SDL_Texture* t = texture_manager.get_resource("test_texture.png");
    REQUIRE(t == nullptr);
  }

  SECTION("load config constructor")
  {
    gk::texture_manager texture_manager{renderer, "texture.cfg"};
    REQUIRE(texture_manager.require_resource("schiggy"));
    SDL_Texture* t = texture_manager.get_resource("schiggy");
    REQUIRE(t != nullptr);
  }

  SECTION("unload texture")
  {
    gk::texture_manager texture_manager{renderer, "texture.cfg"};
    REQUIRE(texture_manager.require_resource("schiggy"));
    SDL_Texture* t = texture_manager.get_resource("schiggy");
    REQUIRE(t != nullptr);
    REQUIRE(texture_manager.release_resource("schiggy"));
    REQUIRE(texture_manager.find("schiggy") == nullptr);
  }

  SECTION("unload multiple texture")
  {
    gk::texture_manager texture_manager{renderer, "texture.cfg"};
    REQUIRE(texture_manager.require_resource("schiggy"));
    REQUIRE(texture_manager.require_resource("schiggy"));
    REQUIRE(texture_manager.find("schiggy")->second == 2);
    REQUIRE(texture_manager.release_resource("schiggy"));
    REQUIRE(texture_manager.find("schiggy"));
    REQUIRE(texture_manager.release_resource("schiggy"));
    REQUIRE(texture_manager.find("schiggy") == nullptr);
  }

  SECTION("purge")
  {
    gk::texture_manager texture_manager{renderer};
    texture_manager.load_config("texture.cfg");
    texture_manager.require_resource("schiggy");
    texture_manager.require_resource("a");
    texture_manager.require_resource("b");
    texture_manager.require_resource("c");
    REQUIRE(texture_manager.find("schiggy") != nullptr);
    REQUIRE(texture_manager.find("a") != nullptr);
    REQUIRE(texture_manager.find("b") != nullptr);
    REQUIRE(texture_manager.find("c") != nullptr);
    texture_manager.purge_resource();
    REQUIRE(texture_manager.find("schiggy") == nullptr);
    REQUIRE(texture_manager.find("a") == nullptr);
    REQUIRE(texture_manager.find("b") == nullptr);
    REQUIRE(texture_manager.find("c") == nullptr);
  }

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
}