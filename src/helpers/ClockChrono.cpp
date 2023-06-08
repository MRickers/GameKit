#include "ClockChrono.hpp"

namespace gk
{

  time_ms gk::ChronoClock::TimePassed()
  {
    return std::chrono::duration_cast<std::chrono::milliseconds>(m_stop_time -
                                                                 m_start_time)
        .count();
  }

  void ChronoClock::Start()
  {
    m_start_time = std::chrono::high_resolution_clock::now();
  }

  void ChronoClock::Stop()
  {
    m_stop_time = std::chrono::high_resolution_clock::now();
  }

  bool ChronoClock::HasPassed(const time_ms delta)
  {
    Stop();
    return TimePassed() >= delta ? true : false;
  }

} // namespace gk
