#include "GameKit/core/TextureManager.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("test", "[textureManager]")
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
}