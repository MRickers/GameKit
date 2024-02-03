#include "GameKit/helpers/ClockChrono.hpp"

namespace gk
{

  time_ms gk::ChronoClock::time_passed()
  {
    return std::chrono::duration_cast<std::chrono::milliseconds>(m_stop_time -
                                                                 m_start_time)
        .count();
  }

  void ChronoClock::start()
  {
    m_start_time = std::chrono::system_clock::now();
  }

  void ChronoClock::stop()
  {
    m_stop_time = std::chrono::system_clock::now();
  }

  time_ms ChronoClock::round()
  {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::system_clock::now() - m_start_time)
        .count();
  }

  bool ChronoClock::has_passed(const time_ms delta)
  {
    return round() >= delta;
  }

} // namespace gk
