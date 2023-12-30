#include "GameKit/helpers/clock_chrono.hpp"

namespace gk
{

  time_ms gk::chrono_clock::time_passed()
  {
    return std::chrono::duration_cast<std::chrono::milliseconds>(m_stop_time -
                                                                 m_start_time)
        .count();
  }

  void chrono_clock::start()
  {
    m_start_time = std::chrono::system_clock::now();
  }

  void chrono_clock::stop()
  {
    m_stop_time = std::chrono::system_clock::now();
  }

  time_ms chrono_clock::round()
  {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::system_clock::now() - m_start_time)
        .count();
  }

  bool chrono_clock::has_passed(const time_ms delta)
  {
    return round() >= delta;
  }

} // namespace gk
