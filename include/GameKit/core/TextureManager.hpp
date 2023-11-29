#pragma once
#include "GameKit/core/ResourceManager.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace gk
{
  class TextureManager : public ResourceManager<TextureManager, SDL_Texture>
  {
  public:
    TextureManager(SDL_Renderer* renderer)
        : ResourceManager()
        , m_renderer{renderer}
    {
    }

    TextureManager(SDL_Renderer* renderer, const std::string& loadFile)
        : ResourceManager{loadFile}
        , m_renderer{renderer}
    {
    }

    SDL_Texture* load(const std::string& path)
    {
      if (SDL_Surface* surface = IMG_Load(path.c_str()); surface != nullptr)
      {
        SDL_Texture* texture =
            SDL_CreateTextureFromSurface(m_renderer, surface);
        SDL_FreeSurface(surface);
        return texture;
      }
      return nullptr;
    }

    void unload(SDL_Texture* texture)
    {
      SDL_DestroyTexture(texture);
    };

  private:
    SDL_Renderer* m_renderer;
  };

} // namespace gk
