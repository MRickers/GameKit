#pragma once
#include "GameKit/helpers/clock_chrono.hpp"
#include "GameKit/helpers/clock_sdl.hpp"

namespace gk
{
  template <typename clock> class timer_interface
  {
  private:
    bool m_started{false};
    clock m_clock{};

  public:
    time_ms round()
    {
      return m_clock.round();
    }
    void reset()
    {
      m_clock.start();
      m_started = true;
    }

    void start()
    {
      if (!m_started)
      {
        m_clock.start();
        m_started = true;
      }
    }

    void stop()
    {
      if (m_started)
      {
        m_started = false;
        m_clock.stop();
      }
    }

    time_ms time_passed()
    {
      return m_clock.time_passed();
    }

    bool is_started() const
    {
      return m_started;
    }

    bool has_passed(time_ms delta)
    {
      return m_clock.has_passed(delta);
    }
  };

  using Timer = timer_interface<chrono_clock>;
  using TimerSdl = timer_interface<sdl_clock>;

} // namespace gk
