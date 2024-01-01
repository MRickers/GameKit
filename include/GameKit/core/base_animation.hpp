#pragma once
#include "GameKit/helpers/types.hpp"
#include <memory>
#include <stdint.h>
#include <string>

namespace gk
{
  class sprite_sheet;

  class base_animation
  {
  public:
    using frame_t = uint16_t;

    explicit base_animation(sprite_sheet* t_sprite_sheet);
    virtual ~base_animation() = default;

    void set_sprite_sheet(sprite_sheet* t_sprite_sheet);
    void set_frame(frame_t t_frame);
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
    frame_t m_frame_current{0};
    frame_t m_frame_start{0};
    frame_t m_frame_end{0};
    frame_t m_frame_row{0};
    int m_frame_action_start{-1};
    int m_frame_action_end{-1};
    time_ms m_frame_time{0};
    time_ms m_elapsed_time{0};
    bool m_loop{false};
    bool m_playing{false};
    std::string m_name{""};

  protected:
    sprite_sheet* m_sprite_sheet{nullptr};
  };
} // namespace gk
