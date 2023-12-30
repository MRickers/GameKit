#include "GameKit/helpers/clock_sdl.hpp"

namespace gk
{

  time_ms gk::sdl_clock::time_passed()
  {
    return m_start_time - m_stop_time;
  }

  void sdl_clock::start()
  {
    m_start_time = SDL_GetTicks();
  }

  void sdl_clock::stop()
  {
    m_stop_time = SDL_GetTicks();
  }

  time_ms sdl_clock::round()
  {
    return SDL_GetTicks() - m_start_time;
  }

  bool sdl_clock::has_passed(const time_ms delta)
  {
    return round() >= delta;
  }

} // namespace gk
