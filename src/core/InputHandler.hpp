#pragma once
#include "vector/Vector2d.hpp"
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include <SDL2/SDL.h>

namespace gk
{
  using Event = SDL_Event;
  using Events = std::vector<Event>;

  struct EventDetails
  {
    std::string id{""};
    Vector2D mouse_pos{0, 0};

    void Reset()
    {
      mouse_pos = {};
    }
  };

  struct EventBinding
  {
    std::string id{""};
    Events events{};
    size_t event_counter{0};
    EventDetails event_details{};
  };
  using EventCallback = std::function<void(const EventDetails&)>;
  class InputHandler
  {
    using Callbacks = std::unordered_map<std::string, EventCallback>;
    using Bindings = std::unordered_map<std::string, EventBinding>;

  public:
    bool AddCallback(const std::string& id, EventCallback);
    bool RemoveCallback(const std::string& id);

    bool AddBinding(const std::string& id, const EventBinding);
    bool RemoveBinding(const std::string& id);

    void HandleEvent(const Event&);
    void Update();

  private:
    Callbacks m_callbacks{};
    Bindings m_bindings{};
    const Uint8* m_keystates{nullptr};

    bool isKeyEvent(const uint32_t event_type);
    bool isMouseEvent(const uint32_t event_type);
    void updateKeyStates();
    bool isKeyDown(SDL_Scancode key) const;
    bool isKeyUp(SDL_Scancode key) const;
    bool keyDownEvent(const uint32_t event_type) const;
    bool keyUpEvent(const uint32_t event_type) const;
  };
} // namespace gk
