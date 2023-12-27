#include "GameKit/core/StateInputHandler.hpp"

namespace gk
{
  void StateInputHandler::setCurrentState(const StateType state)
  {
    m_currentState = state;
  }

  StateType StateInputHandler::currentState() const
  {
    return m_currentState;
  }

  bool StateInputHandler::AddGlobalCallback(const std::string& id,
                                            event_callback callback)
  {
    return m_globalInputs.add_callback(id, callback);
  }

  bool StateInputHandler::RemoveGlobalCallback(const std::string& id)
  {
    return m_globalInputs.remove_callback(id);
  }

  bool StateInputHandler::AddGlobalBinding(const event_binding& binding)
  {
    return m_globalInputs.add_binding(binding);
  }

  bool StateInputHandler::RemoveGlobalBinding(const std::string& id)
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
      if (!m_handlers.try_emplace(state, std::make_unique<input_handler>())
               .second)
      {
        return false;
      }
    }
    return m_handlers[state]->add_callback(id, callback);
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

  bool StateInputHandler::add_binding(const StateType state,
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
    if (m_handlers.find(m_currentState) != m_handlers.end())
    {
      m_handlers[m_currentState]->handle_event(evnt);
    }
    m_globalInputs.handle_event(evnt);
  }
  void StateInputHandler::Update()
  {
    if (m_handlers.find(m_currentState) != m_handlers.end())
    {
      m_handlers[m_currentState]->update();
    }
    m_globalInputs.update();
  }
} // namespace gk
