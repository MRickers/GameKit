#pragma once
#include "GameKit/vector/vector2d.hpp"

#include <functional>
#include <memory>
#include <string>

#include <SDL2/SDL.h>

namespace gk
{

  enum MouseButton
  {
    Nothing = 0,
    Left = 1,
    Middle = 2,
    Right = 3,
    Motion = 4,
  };

  struct event_details
  {
    std::string id;
    vector2d mouse_pos{0, 0};

    void reset() // private
    {
      mouse_pos = {};
    }
  };
  using event_callback = std::function<void(const event_details&)>;
  using keys = std::vector<SDL_Scancode>;
  using mouse_buttons = std::vector<gk::MouseButton>;

  class input_handler
  {
  public:
    input_handler();
    ~input_handler();

    bool add_callback(const std::string& id, event_callback);
    bool remove_callback(const std::string& id);

    bool add_keydown_binding(std::string_view id, gk::keys const& t_keys,
        mouse_buttons t_mouse_buttons = {});
    bool add_keydown_repeat_binding(std::string_view id, keys const& t_keys,
        mouse_buttons t_mouse_buttons = {});

    bool remove_binding(const std::string& id);
    void handle_event(const SDL_Event&);

    void update();

  private:
    struct details;
    std::unique_ptr<details> const m_details;
  };
} // namespace gk
