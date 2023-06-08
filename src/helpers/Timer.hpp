#pragma once
#include "ClockChrono.hpp"

namespace gk
{
  template <typename Clock> class ITimer
  {
  private:
    bool m_started{false};
    Clock m_clock{};

  public:
    void Reset()
    {
      m_clock.Start();
      m_started = true;
    }

    void Start()
    {
      if (!m_started)
      {
        m_clock.Start();
        m_started = true;
      }
    }

    void Stop()
    {
      if (m_started)
      {
        m_started = false;
        m_clock.Stop();
      }
    }

    time_ms TimePassed()
    {
      return m_clock.TimePassed();
    }

    bool IsStarted() const
    {
      return m_started;
    }

    bool HasPassed(time_ms delta)
    {
      return m_clock.HasPassed(delta);
    }
  };

  using Timer = ITimer<ChronoClock>;

} // namespace gk
