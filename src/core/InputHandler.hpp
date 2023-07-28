#pragma once
#include "vector/Vector2d.hpp"
#include <array>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include <SDL2/SDL.h>

namespace gk
{
  enum EventType
  {
    Idle,
    KeyDown = SDL_KEYDOWN,
    KeyUp = SDL_KEYUP,
    MouseMotion = SDL_MOUSEMOTION,
    MouseDown = SDL_MOUSEBUTTONDOWN,
    MouseUp = SDL_MOUSEBUTTONUP,

  };

  enum MouseButton
  {
    Left = 1,
    Middle = 2,
    Right = 3,
    Motion,
  };
  struct Event
  {
    EventType type{Idle};
    SDL_Scancode scancode{SDL_SCANCODE_UNKNOWN};
    MouseButton mouseButton{Left};
  };

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
    bool already_invoked{false};
    size_t event_counter{0};
    EventDetails event_details{};
  };
  using EventCallback = std::function<void(const EventDetails&)>;
  class InputHandler
  {
    using Callbacks = std::unordered_map<std::string, EventCallback>;
    using Bindings = std::unordered_map<std::string, EventBinding>;
    // Index 0 = left mouse button pressed
    // Index 1 = middle mouse button pressed
    // Index 2 = right mouse button pressed
    using MouseButtonState = std::array<bool, 3>;

  public:
    bool AddCallback(const std::string& id, EventCallback);
    bool RemoveCallback(const std::string& id);

    bool AddBinding(const EventBinding&);
    bool RemoveBinding(const std::string& id);

    void HandleEvent(const SDL_Event&);
    void Update();

  private:
    Callbacks m_callbacks{};
    Bindings m_bindings{};
    const Uint8* m_keystates{nullptr};
    MouseButtonState m_mouseEvents{};
    bool m_mouseMotion{false};
    int m_mouseX{0};
    int m_mouseY{0};

    bool isKeyEvent(const uint32_t event_type);
    bool isMouseEvent(const uint32_t event_type);
    void updateMouseStates();
    void updateKeyStates();
    bool isKeyDown(SDL_Scancode key) const;
    bool isKeyUp(SDL_Scancode key) const;
    bool isMouseButtonDown(const MouseButton button) const;
  };
} // namespace gk
