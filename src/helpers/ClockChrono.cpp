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
    m_start_time = std::chrono::system_clock::now();
  }

  void ChronoClock::Stop()
  {
    m_stop_time = std::chrono::system_clock::now();
  }

  time_ms ChronoClock::Round()
  {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::system_clock::now() - m_start_time)
        .count();
  }

  bool ChronoClock::HasPassed(const time_ms delta)
  {
    return Round() >= delta ? true : false;
  }

} // namespace gk
