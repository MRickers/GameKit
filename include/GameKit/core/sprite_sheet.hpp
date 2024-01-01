#pragma once
#include "GameKit/core/base_animation.hpp"
#include "GameKit/core/texture_manager.hpp"
#include "GameKit/helpers/types.hpp"
#include "GameKit/vector/vector2d.hpp"
#include <filesystem>
#include <memory>
#include <string>

class SDL_Texture;
class SDL_Renderer;

namespace gk
{
  class sprite_sheet
  {
    using animations = std::unordered_map<std::string, std::unique_ptr<base_animation>>;

  public:
    enum class direction
    {
      LEFT,
      RIGHT,
    };

    explicit sprite_sheet(std::unique_ptr<gk::texture_manager> &texture_manager);
    ~sprite_sheet();

    void crop_sprite(gk::rect const& t_rect);
    [[maybe_unused]] bool load_sheet(std::filesystem::path const& t_filename);
    void release_sheet();

    void set_sprite_size(gk::vector2d const& t_size);
    void set_sprite_position(gk::vector2d const& t_pos);
    void set_direction(sprite_sheet::direction t_direction);
    bool set_animation(std::string const& t_name, bool t_play,
                       bool t_loop);
    gk::vector2d get_sprite_size() const;
    direction get_direction() const;
    base_animation* get_current_animation() const;
    void update(time_ms t_dt);
    void draw(SDL_Renderer* renderer);

  private:
    std::string m_texture_name;
    SDL_Texture* m_sprite;
    gk::vector2d m_sprite_size;
    // position on render target
    gk::vector2d m_sprite_pos;
    gk::vector2d m_sprite_scale;
    direction m_direction;
    std::string m_animation_type;
    animations m_animations;
    base_animation* m_current_animation{};
    std::unique_ptr<texture_manager>& m_texture_manager;
    // postion on texture
    gk::rect m_clip;
  };

} // namespace gk