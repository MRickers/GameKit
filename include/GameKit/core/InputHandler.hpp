#pragma once
#include "GameKit/vector/Vector2d.hpp"
#include <array>
#include <functional>
#include <memory>
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
    Motion = 4,
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
    size_t event_counter{0};
    EventDetails event_details{};
    bool already_invoked{false};
  };
  using EventCallback = std::function<void(const EventDetails&)>;
  class InputHandler
  {

    using Callbacks = std::unordered_map<std::string, EventCallback>;
    using Bindings = std::unordered_map<std::string, EventBinding>;
    // Index 0 = left mouse button pressed
    // Index 1 = middle mouse button pressed
    // Index 2 = right mouse button pressed
    // Index 4 = motion
    using MouseButtonState = std::array<bool, MouseButton::Motion>;

  public:
    InputHandler();
    ~InputHandler();

    bool AddCallback(const std::string& id, EventCallback);
    bool RemoveCallback(const std::string& id);

    bool AddBinding(const EventBinding&);
    bool RemoveBinding(const std::string& id);

    void HandleEvent(const SDL_Event&);
    void Update();

  private:
    Callbacks m_callbacks{};
    Bindings m_bindings{};

    class Details;
    std::unique_ptr<Details> m_details{nullptr};

    const Uint8* m_keystates{nullptr};
    MouseButtonState m_mouseEvents{};
    bool m_mouseMotion{false};
    int m_mouseX{0};
    int m_mouseY{0};

    bool isKeyEvent(const uint32_t event_type);
    bool isMouseButtonEvent(const uint32_t event_type);
    bool isMouseMotionEvent(const uint32_t event_type);
    void updateMouseStates(const uint32_t event_type);
    void updateKeyStates();
    bool isKeyDown(SDL_Scancode key) const;
    bool isKeyUp(SDL_Scancode key) const;
    bool isMouseButtonDown(const MouseButton button) const;
    bool isMotion() const;

    bool setInvoked(const std::string id);
    bool resetInvoked(const std::string id);
    bool wasInvoked(const std::string& id) const;
  };
} // namespace gk