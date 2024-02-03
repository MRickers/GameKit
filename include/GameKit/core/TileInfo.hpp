//
// Created by max on 02.02.24.
//

#ifndef GAMEKIT_TILE_INFO_HPP
#define GAMEKIT_TILE_INFO_HPP

namespace gk
{
#include "GameKit/core/TextureManager.hpp"
#include "GameKit/vector/Vector2d.hpp"
#include <SDL2/SDL.h>
#include <string>

  namespace gk {

    struct Sheet{
      std::size_t tile_size{32};
      std::size_t tile_width{256};
      std::size_t tile_height{256};
    };
    using TileId = std::size_t;

    struct TileInfo {
      explicit

      SDL_Texture* m_texture{nullptr};
      tile_id m_id{};
      std::string m_name{};
      vector2d m_friction{};
      texture_manager* m_texture_manager{nullptr};
      std::string m_texture{};
    };
  }

} // namespace gk

#endif // GAMEKIT_TILE_INFO_HPP
