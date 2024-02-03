#pragma once
#include "GameKit/helpers/ClockChrono.hpp"
#include "GameKit/helpers/SdlClock.hpp"

namespace gk
{
  template <typename Clock> class TimerInterface
  {
  private:
    bool m_started{false};
    Clock m_clock{};

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

    [[nodiscard]] bool is_started() const
    {
      return m_started;
    }

    bool has_passed(time_ms delta)
    {
      return m_clock.has_passed(delta);
    }
  };

  using Timer = TimerInterface<ChronoClock>;
  using TimerSdl = TimerInterface<SdlClock>;

} // namespace gk
