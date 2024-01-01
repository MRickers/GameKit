#include "GameKit/core/sprite_sheet.hpp"
#include "GameKit/core/animiation_directional.hpp"
#include "GameKit/helpers/game_exception.hpp"
#include <fstream>

gk::sprite_sheet::sprite_sheet(
    std::unique_ptr<gk::texture_manager>& texture_manager)
    : m_sprite{nullptr}
    , m_sprite_size{}
    , m_sprite_scale{1.f, 1.f}
    , m_direction{sprite_sheet::direction::RIGHT}
    , m_texture_manager{texture_manager}
    , m_clip{}
{
}

gk::sprite_sheet::~sprite_sheet()
{
  release_sheet();
}

void gk::sprite_sheet::crop_sprite(gk::rect const& t_rect)
{
  m_clip = t_rect;
}

void gk::sprite_sheet::release_sheet()
{
  if (!m_texture_manager->release_resource(m_texture_name))
  {
    SDL_LogWarn(SDL_LOG_PRIORITY_WARN, "could not release resource: %s",
                m_texture_name.c_str());
    return;
  }
  m_current_animation = nullptr;
  while (m_animations.begin() != m_animations.end())
  {
    m_animations.erase(m_animations.begin());
  }
}

void gk::sprite_sheet::set_sprite_size(gk::vector2d const& t_size)
{
  m_sprite_size = t_size;
  m_clip.size = t_size;
}

void gk::sprite_sheet::set_sprite_position(gk::vector2d const& t_pos)
{
  m_sprite_pos = t_pos;
}

void gk::sprite_sheet::set_direction(sprite_sheet::direction t_direction)
{
  if (m_direction == t_direction)
  {
    return;
  }
  m_direction = t_direction;
  // TODO: crop_animation?!
}

gk::vector2d gk::sprite_sheet::get_sprite_size() const
{
  return m_sprite_size;
}

gk::sprite_sheet::direction gk::sprite_sheet::get_direction() const
{
  return m_direction;
}

[[maybe_unused]] bool gk::sprite_sheet::set_animation(std::string const& t_name,
                                                      bool const t_play,
                                                      bool const t_loop)
{
  if (auto itr = m_animations.find(t_name); itr != m_animations.end())
  {
    auto const name = itr->first;
    auto const animation = itr->second.get();

    if (!animation)
    {
      SDL_LogError(SDL_LOG_PRIORITY_ERROR, "invalid animation");
      return false;
    }
    if (animation == m_current_animation)
    {
      return false;
    }
    if (m_current_animation)
    {
      m_current_animation->stop();
    }
    m_current_animation = animation;
    m_current_animation->set_looping(t_loop);
    if (t_play)
    {
      m_current_animation->play();
    }
    m_current_animation->crop_sprite();
    return true;
  }
  return false;
}

void gk::sprite_sheet::update(time_ms t_dt)
{
  m_current_animation->update(t_dt);
}

void gk::sprite_sheet::draw(SDL_Renderer* renderer)
{
  const SDL_Rect src{
      m_clip.pos.GetX<int>(),
      m_clip.pos.GetY<int>(),
      m_clip.size.GetX<int>(),
      m_clip.size.GetY<int>(),
  };

  const SDL_Rect dst = {
      m_sprite_pos.GetX<int>(),
      m_sprite_pos.GetY<int>(),
      m_clip.size.GetX<int>(),
      m_clip.size.GetY<int>(),
  };

  if (auto ret = SDL_RenderCopy(renderer, m_sprite, &src, &dst); ret != 0)
  {
    SDL_LogError(SDL_LOG_PRIORITY_WARN, "could not render sprite: %d", ret);
  }
}

bool gk::sprite_sheet::load_sheet(std::filesystem::path const& t_filename)
{
  std::ifstream ifs;
  ifs.open(t_filename.string());
  if (ifs.is_open())
  {
    release_sheet();
    std::string line;
    while (std::getline(ifs, line))
    {
      if (line[0] == '|')
      {
        continue;
      }

      std::stringstream key_stream(line);
      std::string type;
      key_stream >> type;

      if (type == "Texture")
      {
        if (!m_texture_name.empty())
        {
          SDL_LogWarn(SDL_LOG_PRIORITY_WARN, "duplicated texture entries");
          continue;
        }
        std::string texture;
        key_stream >> texture;
        if (!m_texture_manager->require_resource(texture))
        {
          SDL_LogError(SDL_LOG_PRIORITY_ERROR, "could not set up texture: %s",
                       texture.c_str());
          continue;
        }
        m_texture_name = texture;
        m_sprite = m_texture_manager->get_resource(m_texture_name);
      }
      else if (type == "Size")
      {
        int x, y;
        key_stream >> x >> y;
        m_sprite_size = {x, y};
        set_sprite_size(m_sprite_size);
      }
      else if (type == "Scale")
      {
        int x, y;
        key_stream >> x >> y;
        m_sprite_scale = {x, y};
      }
      else if (type == "AnimationType")
      {
        key_stream >> m_animation_type;
      }
      else if (type == "Animation")
      {
        std::string name;
        key_stream >> name;

        if (m_animations.find(name) != m_animations.end())
        {
          SDL_LogWarn(SDL_LOG_PRIORITY_WARN,
                      "duplicated animation %s in file %s", name.c_str(),
                      t_filename.string().c_str());
          continue;
        }
        if (m_animation_type == "Directional")
        {
          auto animation = std::make_unique<animation_directional>(this);
          animation->parse_file_input(key_stream);
          animation->set_name(name);
          animation->reset();
          m_animations.emplace(name, std::move(animation));
          if (m_current_animation)
          {
            continue;
          }
          m_current_animation = m_animations[name].get();
          m_current_animation->play();
        }
        else
        {
          SDL_LogWarn(SDL_LOG_PRIORITY_WARN, "invalid animation type: %s",
                      m_animation_type.c_str());
          continue;
        }
      }
    }
    ifs.close();
    return true;
  }
  return false;
}