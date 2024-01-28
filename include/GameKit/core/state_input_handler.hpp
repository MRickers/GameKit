#pragma once
#include "GameKit/core/input_handler.hpp"
#include <unordered_map>

enum class StateType;

namespace gk
{

  class state_input_handler
  {
    using input_handler_owner = std::unique_ptr<input_handler>;
    using input_handlers = std::unordered_map<StateType, input_handler_owner>;

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
    input_handlers m_handlers;
    input_handler m_globalInputs;
    StateType m_current_state;
  };

} // namespace gk
