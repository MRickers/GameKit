#pragma once
#include "GameKit/vector/vector2d.hpp"
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
  
  struct event
  {
    enum class on_keyhold_behaviour
    {
      invoke_once,
      invoke_repeat,
    };
    EventType type{Idle};
    // only relevant for keydown events. If multiple keys are used and one is
    // set to "invoke_repeat" this behaviour is used for that binding
    on_keyhold_behaviour on_keyhold{on_keyhold_behaviour::invoke_once};
    SDL_Scancode scancode{SDL_SCANCODE_UNKNOWN};
    MouseButton mouse_button{Left};
  };

  using event_array = std::vector<event>;

  struct event_details
  {
    std::string id{""};
    vector2d mouse_pos{0, 0};

    void reset() // private
    {
      mouse_pos = {};
    }
  };

  struct event_binding
  {
    std::string id{""};
    event_array events{};
    size_t event_counter{0};  // TODO: private
    event_details details{};  // TODO: private
    bool already_invoked{false};  // TODO: private
  };

  event_binding make_keydown_binding(std::string_view const& t_id, SDL_Scancode t_scancode);
  event_binding make_keydown_repeat_binding(std::string_view const& t_id, SDL_Scancode t_scancode);

  using event_callback = std::function<void(const event_details&)>;

  class input_handler
  {
    using callbacks = std::unordered_map<std::string, event_callback>;
    using bindings = std::unordered_map<std::string, event_binding>;
    // Index 0 = left mouse button pressed
    // Index 1 = middle mouse button pressed
    // Index 2 = right mouse button pressed
    // Index 4 = motion
    using MouseButtonState = std::array<bool, MouseButton::Motion>;

  public:
    input_handler() = default;

    bool add_callback(const std::string& id, event_callback);
    bool remove_callback(const std::string& id);

    bool add_binding(const event_binding&);
    bool remove_binding(const std::string& id);

    void handle_event(const SDL_Event&);
    void update();

  private:
    callbacks m_callbacks{};
    bindings m_bindings{};

    std::unordered_map<std::string, bool> m_details{};

    const Uint8* m_keystates{nullptr};
    MouseButtonState m_mouseEvents{};
    bool m_mouseMotion{false};
    int m_mouseX{0};
    int m_mouseY{0};

    bool isKeyEvent(uint32_t event_type);
    bool isMouseButtonEvent(uint32_t event_type);
    bool isMouseMotionEvent(uint32_t event_type);
    void updateMouseStates(uint32_t event_type);
    void updateKeyStates();
    bool isKeyDown(SDL_Scancode key) const;
    bool isKeyUp(SDL_Scancode key) const;
    bool isMouseButtonDown(MouseButton button) const;
    bool isMotion() const;

    bool setInvoked(std::string const& id);
    bool resetInvoked(std::string const& id);
    bool wasInvoked(const std::string& id) const;
  };
} // namespace gk
