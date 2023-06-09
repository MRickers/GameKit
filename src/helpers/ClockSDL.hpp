#pragma once
#include "helpers/Types.hpp"
#include <SDL2/SDL.h>

namespace gk
{
  class SDLClock
  {
  public:
    time_ms TimePassed();
    void Start();
    void Stop();
    time_ms Round();
    bool HasPassed(const time_ms delta);

  private:
    Uint32 m_start_time{0};
    Uint32 m_stop_time{0};
  };

} // namespace gk
