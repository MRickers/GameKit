#include "GameKit/core/state_input_handler.hpp"

namespace gk
{
  void state_input_handler::set_current_state(const StateType state)
  {
    m_current_state = state;
  }

  StateType state_input_handler::current_state() const
  {
    return m_current_state;
  }

  bool state_input_handler::add_global_callback(const std::string& id,
                                                event_callback callback)
  {
    return m_globalInputs.add_callback(id, callback);
  }

  bool state_input_handler::remove_global_callback(const std::string& id)
  {
    return m_globalInputs.remove_callback(id);
  }

  bool state_input_handler::add_global_binding(const event_binding& binding)
  {
    return m_globalInputs.add_binding(binding);
  }

  bool state_input_handler::remove_global_binding(const std::string& id)
  {
    return m_globalInputs.remove_binding(id);
  }

  bool state_input_handler::add_callback(const StateType state,
                                         const std::string& id,
                                         event_callback callback)
  {
    if (auto foundState = m_handlers.find(state);
        foundState == m_handlers.end())
    {
      if (!m_handlers.try_emplace(state, std::make_unique<input_handler>())
               .second)
      {
        return false;
      }
    }
    return m_handlers[state]->add_callback(id, callback);
  }

  bool state_input_handler::remove_callback(const StateType state,
                                            const std::string& id)
  {
    if (auto foundState = m_handlers.find(state);
        foundState != m_handlers.end())
    {
      return m_handlers[state]->remove_callback(id);
    }
    return false;
  }

  bool state_input_handler::add_binding(const StateType state,
                                        const event_binding& binding)
  {
    if (auto foundState = m_handlers.find(state);
        foundState == m_handlers.end())
    {
      if (!m_handlers.try_emplace(state, std::make_unique<input_handler>())
               .second)
      {
        return false;
      }
    }
    return m_handlers[state]->add_binding(binding);
  }

  bool state_input_handler::remove_binding(const StateType state,
                                           const std::string& id)
  {
    if (auto foundState = m_handlers.find(state);
        foundState != m_handlers.end())
    {
      return m_handlers[state]->remove_binding(id);
    }
    return false;
  }

  void state_input_handler::handle_event(const SDL_Event& evnt)
  {
    if (m_handlers.find(m_current_state) != m_handlers.end())
    {
      m_handlers[m_current_state]->handle_event(evnt);
    }
    m_globalInputs.handle_event(evnt);
  }
  void state_input_handler::update()
  {
    if (m_handlers.find(m_current_state) != m_handlers.end())
    {
      m_handlers[m_current_state]->update();
    }
    m_globalInputs.update();
  }
} // namespace gk
