#include "GameKit/helpers/ClockSDL.hpp"

namespace gk
{

  time_ms gk::SDLClock::TimePassed()
  {
    return m_start_time - m_stop_time;
  }

  void SDLClock::Start()
  {
    m_start_time = SDL_GetTicks();
  }

  void SDLClock::Stop()
  {
    m_stop_time = SDL_GetTicks();
  }

  time_ms SDLClock::Round()
  {
    return SDL_GetTicks() - m_start_time;
  }

  bool SDLClock::HasPassed(const time_ms delta)
  {
    return Round() >= delta ? true : false;
  }

} // namespace gk
