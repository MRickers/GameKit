#include "GameKit/core/StateInputHandler.hpp"

namespace gk
{
  void StateInputHandler::set_current_state(const StateType state)
  {
    m_current_state = state;
  }

  StateType StateInputHandler::current_state() const
  {
    return m_current_state;
  }

  bool StateInputHandler::add_global_callback(const std::string& id,
                                                event_callback callback)
  {
    return m_globalInputs.add_callback(id, callback);
  }

  bool StateInputHandler::remove_global_callback(const std::string& id)
  {
    return m_globalInputs.remove_callback(id);
  }

  bool StateInputHandler::remove_global_binding(const std::string& id)
  {
    return m_globalInputs.remove_binding(id);
  }

  bool StateInputHandler::add_callback(const StateType state,
                                         const std::string& id,
                                         event_callback callback)
  {
    if (auto foundState = m_handlers.find(state);
        foundState == m_handlers.end())
    {
      if (!m_handlers.try_emplace(state, std::make_unique<InputHandler>())
               .second)
      {
        return false;
      }
    }
    return m_handlers[state]->add_callback(id, std::move(callback));
  }

  bool StateInputHandler::remove_callback(const StateType state,
                                            const std::string& id)
  {
    if (auto foundState = m_handlers.find(state);
        foundState != m_handlers.end())
    {
      return m_handlers[state]->remove_callback(id);
    }
    return false;
  }

  bool StateInputHandler::remove_binding(const StateType state,
                                           const std::string& id)
  {
    if (auto foundState = m_handlers.find(state);
        foundState != m_handlers.end())
    {
      return m_handlers[state]->remove_binding(id);
    }
    return false;
  }

  void StateInputHandler::handle_event(const SDL_Event& evnt)
  {
    if (m_handlers.find(m_current_state) != m_handlers.end())
    {
      m_handlers[m_current_state]->handle_event(evnt);
    }
    m_globalInputs.handle_event(evnt);
  }
  void StateInputHandler::update()
  {
    if (m_handlers.find(m_current_state) != m_handlers.end())
    {
      m_handlers[m_current_state]->update();
    }
    m_globalInputs.update();
  }
  bool StateInputHandler::add_global_keydown_binding(
      std::string_view id, keys const& t_keys, mouse_buttons const& t_mouse_buttons)
  {
    return m_globalInputs.add_keydown_binding(id, t_keys, t_mouse_buttons);
  }
  bool StateInputHandler::add_global_keydown_repeat_binding(
      std::string_view id, keys const& t_keys, mouse_buttons const& t_mouse_button)
  {
    return m_globalInputs.add_keydown_repeat_binding(id, t_keys,
                                                     t_mouse_button);
  }
  bool StateInputHandler::add_keydown_binding(StateType t_state,
                                                std::string_view t_id,
                                                keys const& t_keys,
                                                mouse_buttons const& t_mouse_buttons)
  {
    if (auto foundState = m_handlers.find(t_state);
        foundState == m_handlers.end())
    {
      if (!m_handlers.try_emplace(t_state, std::make_unique<InputHandler>())
               .second)
      {
        return false;
      }
    }
    return m_handlers[t_state]->add_keydown_binding(t_id, t_keys,t_mouse_buttons);
  }
  bool StateInputHandler::add_keydown_repeat_binding(StateType t_state,
                                                       std::string_view t_id,
                                                       keys const& t_keys,
                                                       mouse_buttons const& t_mouse_buttons)
  {
    if (auto foundState = m_handlers.find(t_state);
        foundState == m_handlers.end())
    {
      if (!m_handlers.try_emplace(t_state, std::make_unique<InputHandler>())
               .second)
      {
        return false;
      }
    }
    return m_handlers[t_state]->add_keydown_repeat_binding(t_id, t_keys, t_mouse_buttons);
  }
} // namespace gk
