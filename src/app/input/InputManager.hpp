#pragma once
#include <SDL2/SDL.h>
#include <array>
#include <functional>
#include <string>
#include <unordered_map>

namespace gk
{
  class EventDetails;
  using InputCallback = std::function<void>(const EventDetails&);

  // Details of occurred event
  struct EventDetails
  {
    int x{0};
    int y{0};
  };

  // Details for event to occure
  struct InputDetails
  {
    std::string name{""};
    SDL_EventType event_type{};
    std::array<SDL_Scancode, 4> keys{};
  };

  class InputManager
  {
    using Callbacks =
        std::unordered_map<SDL_EventType, std::vector<InputCallback>>;

  public:
    void addCallback(const InputDetails& details, InputCallback callback);
    void removeCallback(const std::string& name);
    void handleEvents();

  private:
    Callbacks m_callbacks{};
  };

} // namespace gk
