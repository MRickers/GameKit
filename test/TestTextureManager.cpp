#include "GameKit/core/TextureManager.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Interface", "[textureManager]")
{
  SDL_Window* window =
      SDL_CreateWindow("Texturemanager test", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 475, 475, 0);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

  SECTION("Default construct")
  {
    gk::TextureManager textureManager{renderer};
    SDL_Texture* t = textureManager.load("test_texture.png");
    REQUIRE(t != nullptr);
  }

  SECTION("load invalid config constructor")
  {
    gk::TextureManager textureManager{renderer, "test_texture.png"};
    SDL_Texture* t = textureManager.getResource("test_texture.png");
    REQUIRE(t == nullptr);
  }

  SECTION("load config constructor")
  {
    gk::TextureManager textureManager{renderer, "texture.cfg"};
    REQUIRE(textureManager.requireResource("schiggy"));
    SDL_Texture* t = textureManager.getResource("schiggy");
    REQUIRE(t != nullptr);
  }

  SECTION("unload texture")
  {
    gk::TextureManager textureManager{renderer, "texture.cfg"};
    REQUIRE(textureManager.requireResource("schiggy"));
    SDL_Texture* t = textureManager.getResource("schiggy");
    REQUIRE(t != nullptr);
    REQUIRE(textureManager.releaseResource("schiggy"));
    REQUIRE(textureManager.find("schiggy") == nullptr);
  }

  SECTION("unload multiple texture")
  {
    gk::TextureManager textureManager{renderer, "texture.cfg"};
    REQUIRE(textureManager.requireResource("schiggy"));
    REQUIRE(textureManager.requireResource("schiggy"));
    REQUIRE(textureManager.find("schiggy")->second == 2);
    REQUIRE(textureManager.releaseResource("schiggy"));
    REQUIRE(textureManager.find("schiggy"));
    REQUIRE(textureManager.releaseResource("schiggy"));
    REQUIRE(textureManager.find("schiggy") == nullptr);
  }

  SECTION("purge")
  {
    gk::TextureManager textureManager{renderer, "texture.cfg"};
    textureManager.loadConfig("texture.cfg");
    textureManager.requireResource("schiggy");
    textureManager.requireResource("a");
    textureManager.requireResource("b");
    textureManager.requireResource("c");
    REQUIRE(textureManager.find("schiggy") != nullptr);
    REQUIRE(textureManager.find("a") != nullptr);
    REQUIRE(textureManager.find("b") != nullptr);
    REQUIRE(textureManager.find("c") != nullptr);
    textureManager.purgeResources();
    REQUIRE(textureManager.find("schiggy") == nullptr);
    REQUIRE(textureManager.find("a") == nullptr);
    REQUIRE(textureManager.find("b") == nullptr);
    REQUIRE(textureManager.find("c") == nullptr);
  }

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
}