#pragma once
#include "GameKit/helpers/Types.hpp"
#include <memory>
#include <stdint.h>
#include <string>

namespace gk
{
  class SpriteSheet;

  class BaseAnimation
  {
  public:
    using FrameCount = uint16_t;

    explicit BaseAnimation(SpriteSheet* t_sprite_sheet);
    virtual ~BaseAnimation() = default;

    void set_sprite_sheet(SpriteSheet* t_sprite_sheet);
    void set_frame(FrameCount t_frame);
    bool is_in_action() const;
    void set_name(const std::string& t_name);
    void set_looping(bool t_loop);
    void play();
    void pause();
    void stop();
    void reset();

    virtual void update(time_ms t_dt);

    virtual void frame_step() = 0;
    virtual void crop_sprite() = 0;

  protected:
    FrameCount m_frame_current{0};
    FrameCount m_frame_start{0};
    FrameCount m_frame_end{0};
    FrameCount m_frame_row{0};
    int m_frame_action_start{-1};
    int m_frame_action_end{-1};
    time_ms m_frame_time{0};
    time_ms m_elapsed_time{0};
    bool m_loop{false};
    bool m_playing{false};
    std::string m_name{};

  protected:
    SpriteSheet* m_sprite_sheet{nullptr};
  };
} // namespace gk
