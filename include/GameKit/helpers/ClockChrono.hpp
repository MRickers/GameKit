#pragma once
#include "GameKit/helpers/Types.hpp"
#include <chrono>

namespace gk
{
  class ChronoClock
  {
  public:
    time_ms time_passed();
    void start();
    void stop();
    time_ms round();
    bool has_passed(const time_ms delta);

  private:
    std::chrono::system_clock::time_point m_start_time{
        std::chrono::high_resolution_clock::now()};
    std::chrono::system_clock::time_point m_stop_time{
        std::chrono::high_resolution_clock::now()};
  };

} // namespace gk
