#include "GameKit/core/BaseAnimation.hpp"

gk::BaseAnimation::BaseAnimation(SpriteSheet* t_sprite_sheet)
    : m_frame_current{0}
    , m_frame_start{0}
    , m_frame_end{0}
    , m_frame_row{0}
    , m_frame_action_start{-1}
    , m_frame_action_end{-1}
    , m_frame_time{0}
    , m_elapsed_time{0}
    , m_loop{false}
    , m_playing{false}
    , m_name{}
    , m_sprite_sheet{t_sprite_sheet}
{
}

void gk::BaseAnimation::set_sprite_sheet(SpriteSheet* t_sprite_sheet)
{
  if(t_sprite_sheet) {
    m_sprite_sheet = t_sprite_sheet;
  }
}

void gk::BaseAnimation::set_frame(FrameCount t_frame)
{
  if (t_frame >= m_frame_start && t_frame <= m_frame_end)
  {
    m_frame_current = t_frame;
  }
}

void gk::BaseAnimation::set_looping(bool t_loop)
{
  m_loop = t_loop;
}

bool gk::BaseAnimation::is_in_action() const
{
  if (m_frame_action_start == -1 || m_frame_action_end == -1)
  {
    return true;
  }

  return (m_frame_current >= m_frame_action_start &&
          m_frame_current <= m_frame_action_end);
}

void gk::BaseAnimation::play()
{
  m_playing = true;
}

void gk::BaseAnimation::pause()
{
  m_playing = false;
}

void gk::BaseAnimation::stop()
{
  pause();
  reset();
}

void gk::BaseAnimation::reset()
{
  m_frame_current = m_frame_start;
  m_elapsed_time = 0.f;
  crop_sprite();
}

void gk::BaseAnimation::update(time_ms t_dt)
{
  if (!m_playing)
  {
    return;
  }
  m_elapsed_time += t_dt;
  if (m_elapsed_time < m_frame_time)
  {
    return;
  }
  frame_step();
  crop_sprite();
  m_elapsed_time = 0;
}
void gk::BaseAnimation::set_name(const std::string& t_name)
{
  m_name = t_name;
}
