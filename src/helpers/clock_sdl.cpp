#include "GameKit/helpers/SdlClock.hpp"

namespace gk
{

  time_ms gk::SdlClock::time_passed()
  {
    return m_start_time - m_stop_time;
  }

  void SdlClock::start()
  {
    m_start_time = SDL_GetTicks();
  }

  void SdlClock::stop()
  {
    m_stop_time = SDL_GetTicks();
  }

  time_ms SdlClock::round()
  {
    return SDL_GetTicks() - m_start_time;
  }

  bool SdlClock::has_passed(const time_ms delta)
  {
    return round() >= delta;
  }

} // namespace gk
