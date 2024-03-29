#pragma once
#include "GameKit/helpers/Types.hpp"
#include <SDL2/SDL.h>

namespace gk
{
  class SdlClock
  {
  public:
    time_ms time_passed();
    void start();
    void stop();
    time_ms round();
    bool has_passed(const time_ms delta);

  private:
    Uint32 m_start_time{0};
    Uint32 m_stop_time{0};
  };

} // namespace gk
