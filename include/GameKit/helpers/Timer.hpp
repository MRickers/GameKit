#pragma once
#include "ClockChrono.hpp"
#include "ClockSDL.hpp"

namespace gk
{
  template <typename Clock> class ITimer
  {
  private:
    bool m_started{false};
    Clock m_clock{};

  public:
    time_ms Round()
    {
      return m_clock.Round();
    }
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
  using TimerSdl = ITimer<SDLClock>;

} // namespace gk
