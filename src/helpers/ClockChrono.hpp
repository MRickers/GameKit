#pragma once
#include "helpers/Types.hpp"
#include <chrono>

namespace gk
{
  class ChronoClock
  {
  public:
    time_ms TimePassed();
    void Start();
    void Stop();
    bool HasPassed(const time_ms delta);

  private:
    std::chrono::high_resolution_clock::time_point m_start_time{
        std::chrono::high_resolution_clock::now()};
    std::chrono::high_resolution_clock::time_point m_stop_time{
        std::chrono::high_resolution_clock::now()};
  };

} // namespace gk
