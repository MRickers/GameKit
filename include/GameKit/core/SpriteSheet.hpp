#pragma once
#include "GameKit/core/BaseAnimation.hpp"
#include "GameKit/core/TextureManager.hpp"
#include "GameKit/helpers/Types.hpp"
#include "GameKit/vector/Vector2d.hpp"
#include <filesystem>
#include <memory>
#include <string>

class SDL_Texture;
class SDL_Renderer;

namespace gk
{
  class SpriteSheet
  {
    using Animations = std::unordered_map<std::string, std::unique_ptr<BaseAnimation>>;

  public:
    enum class Direction
    {
      LEFT,
      RIGHT,
    };

    explicit SpriteSheet(std::unique_ptr<gk::TextureManager> &texture_manager);
    ~SpriteSheet();

    void crop_sprite(gk::Rect const& t_rect);
    bool load_sheet(std::filesystem::path const& t_filename);
    void release_sheet();

    void set_sprite_size(gk::Vector2d const& t_size);
    void set_sprite_position(gk::Vector2d const& t_pos);
    void set_direction(SpriteSheet::Direction t_direction);
    bool set_animation(std::string const& t_name, bool t_play,
                       bool t_loop);
    gk::Vector2d get_sprite_size() const;
    Direction get_direction() const;
    BaseAnimation* get_current_animation() const;
    void update(time_ms t_dt);
    void draw(SDL_Renderer* renderer);

  private:
    std::string m_texture_name;
    SDL_Texture* m_sprite;
    gk::Vector2d m_sprite_size;
    // position on render target
    gk::Vector2d m_sprite_pos;
    gk::Vector2d m_sprite_scale;
    Direction m_direction;
    std::string m_animation_type;
    Animations m_animations;
    BaseAnimation* m_current_animation{};
    std::unique_ptr<TextureManager>& m_texture_manager;
    // postion on texture
    gk::Rect m_clip;
  };

} // namespace gk