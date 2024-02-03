#include "GameKit/core/AnimiationDirectional.hpp"
#include "GameKit/core/SpriteSheet.hpp"
#include "GameKit/helpers/GameException.hpp"

void gk::AnimationDirectional::frame_step()
{
  if (m_frame_start < m_frame_end)
  {
    ++m_frame_current;
  }
  else
  {
    --m_frame_current;
  }

  if ((m_frame_start < m_frame_end && m_frame_current > m_frame_end) ||
      (m_frame_start > m_frame_end && m_frame_current < m_frame_end))
  {
    if (m_loop)
    {
      m_frame_current = m_frame_start;
      return;
    }
    m_frame_current = m_frame_action_end;
    pause();
  }
}

void gk::AnimationDirectional::crop_sprite()
{
  if(not m_sprite_sheet) {
    throw gk::GameException("invalid sprite_sheet pointer", 1000);
  }
  auto const size_x = m_sprite_sheet->get_sprite_size().GetX<uint16_t>();
  auto const size_y = m_sprite_sheet->get_sprite_size().GetY<uint16_t>();

  gk::Rect rect{{size_x * m_frame_current,
                 size_y * (m_frame_row + static_cast<FrameCount>(
                                             m_sprite_sheet->get_direction()))},
                {size_x, size_y}};
  m_sprite_sheet->crop_sprite(rect);
}
gk::AnimationDirectional::AnimationDirectional(SpriteSheet* tSpriteSheet)
    : BaseAnimation(tSpriteSheet)
{
}
bool gk::AnimationDirectional::parse_file_input(std::stringstream& t_str)
{
  t_str >> m_frame_start >> m_frame_end >> m_frame_row >> m_frame_time >>
      m_frame_action_start >> m_frame_action_end;
  if (t_str.fail())
  {
    return false;
  }
  return true;
}
