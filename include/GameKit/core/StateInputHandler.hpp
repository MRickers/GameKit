#pragma once
#include "GameKit/core/InputHandler.hpp"
#include <unordered_map>

enum class StateType;

namespace gk
{

  class StateInputHandler
  {
    using InputHandlerOwner = std::unique_ptr<InputHandler>;
    using InputHandlers = std::unordered_map<StateType, InputHandlerOwner>;

  public:
    void set_current_state(const StateType state);
    StateType current_state() const;

    bool add_global_keydown_binding(std::string_view id, keys const&,
                                    mouse_buttons const& t_mouse_buttons = {});
    bool
    add_global_keydown_repeat_binding(std::string_view id, keys const&,
                                      mouse_buttons const& t_mouse_button = {});

    bool add_global_callback(const std::string& id, event_callback);
    bool remove_global_callback(const std::string& id);

    bool remove_global_binding(const std::string& id);

    bool add_keydown_binding(StateType, std::string_view id, keys const&,
                             mouse_buttons const & t_mouse_buttons= {});
    bool add_keydown_repeat_binding(StateType, std::string_view id, keys const&,
                                    mouse_buttons const & t_mouse_buttons= {});
    bool add_callback(const StateType state, const std::string& id,
                      event_callback);
    bool remove_callback(const StateType state, const std::string& id);

    //    bool add_binding(const StateType state, const event_binding&);
    bool remove_binding(const StateType state, const std::string& id);

    void handle_event(const SDL_Event&);
    void update();

  private:
    InputHandlers m_handlers;
    InputHandler m_globalInputs;
    StateType m_current_state;
  };

} // namespace gk
