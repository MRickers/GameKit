#include "GameKit/core/AnimiationDirectional.hpp"
#include "GameKit/core/SpriteSheet.hpp"
#include "GameKit/helpers/GameException.hpp"
#include <fstream>

gk::SpriteSheet::SpriteSheet(
    std::unique_ptr<gk::TextureManager>& texture_manager)
    : m_sprite{nullptr}
    , m_sprite_size{}
    , m_sprite_scale{1.f, 1.f}
    , m_direction{SpriteSheet::Direction::LEFT}
    , m_texture_manager{texture_manager}
    , m_clip{}
{
}

gk::SpriteSheet::~SpriteSheet()
{
  release_sheet();
}

void gk::SpriteSheet::crop_sprite(gk::Rect const& t_rect)
{
  m_clip = t_rect;
}

void gk::SpriteSheet::release_sheet()
{
  if (!m_texture_manager->release_resource(m_texture_name))
  {
    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "could not release resource: %s",
                m_texture_name.c_str());
    return;
  }
  m_current_animation = nullptr;
  while (m_animations.begin() != m_animations.end())
  {
    m_animations.erase(m_animations.begin());
  }
}

void gk::SpriteSheet::set_sprite_size(gk::Vector2d const& t_size)
{
  m_sprite_size = t_size;
  m_clip.size = t_size;
}

void gk::SpriteSheet::set_sprite_position(gk::Vector2d const& t_pos)
{
  m_sprite_pos = t_pos;
}

void gk::SpriteSheet::set_direction(SpriteSheet::Direction t_direction)
{
  if (m_direction == t_direction)
  {
    return;
  }
  m_direction = t_direction;
  m_current_animation->crop_sprite();
}

gk::Vector2d gk::SpriteSheet::get_sprite_size() const
{
  return m_sprite_size;
}

gk::SpriteSheet::Direction gk::SpriteSheet::get_direction() const
{
  return m_direction;
}

bool gk::SpriteSheet::set_animation(std::string const& t_name,
                                     bool const t_play, bool const t_loop)
{
  if (auto itr = m_animations.find(t_name); itr != m_animations.end())
  {
    auto const animation = itr->second.get();

    if (!animation)
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "invalid animation");
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

void gk::SpriteSheet::update(time_ms t_dt)
{
  m_current_animation->update(t_dt);
}

void gk::SpriteSheet::draw(SDL_Renderer* renderer)
{
  const SDL_Rect src{
      m_clip.pos.GetX<int>(),
      m_clip.pos.GetY<int>(),
      m_clip.size.GetX<int>(),
      m_clip.size.GetY<int>(),
  };

  const SDL_Rect dest = {
      m_sprite_pos.GetX<int>(),
      m_sprite_pos.GetY<int>(),
      m_clip.size.GetX<int>(),
      m_clip.size.GetY<int>(),
  };

  if (auto ret = SDL_RenderCopy(renderer, m_sprite, &src, &dest); ret != 0)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "could not render sprite: %s",
                 SDL_GetError());
  }
}

bool gk::SpriteSheet::load_sheet(std::filesystem::path const& t_filename)
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
          SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                      "duplicated texture entries");
          continue;
        }
        std::string texture;
        key_stream >> texture;
        if (!m_texture_manager->require_resource(texture))
        {
          SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                       "could not set up texture: %s", texture.c_str());
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
          SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                      "duplicated animation %s in file %s", name.c_str(),
                      t_filename.string().c_str());
          continue;
        }
        if (m_animation_type == "Directional")
        {
          auto animation = std::make_unique<AnimationDirectional>(this);
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
          SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                      "invalid animation type: %s", m_animation_type.c_str());
          continue;
        }
      }
    }
    ifs.close();
    return true;
  }
  return false;
}
gk::BaseAnimation* gk::SpriteSheet::get_current_animation() const
{
  return m_current_animation;
}
