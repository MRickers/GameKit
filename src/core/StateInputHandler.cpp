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
                                            EventCallback callback)
  {
    return m_globalInputs.AddCallback(id, callback);
  }

  bool StateInputHandler::RemoveGlobalCallback(const std::string& id)
  {
    return m_globalInputs.RemoveCallback(id);
  }

  bool StateInputHandler::AddGlobalBinding(const EventBinding& binding)
  {
    return m_globalInputs.AddBinding(binding);
  }

  bool StateInputHandler::RemoveGlobalBinding(const std::string& id)
  {
    return m_globalInputs.RemoveBinding(id);
  }

  bool StateInputHandler::AddCallback(const StateType state,
                                      const std::string& id,
                                      EventCallback callback)
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
    return m_handlers[state]->AddCallback(id, callback);
  }

  bool StateInputHandler::RemoveCallback(const StateType state,
                                         const std::string& id)
  {
    if (auto foundState = m_handlers.find(state);
        foundState != m_handlers.end())
    {
      return m_handlers[state]->RemoveCallback(id);
    }
    return false;
  }

  bool StateInputHandler::AddBinding(const StateType state,
                                     const EventBinding& binding)
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
    return m_handlers[state]->AddBinding(binding);
  }

  bool StateInputHandler::RemoveBinding(const StateType state,
                                        const std::string& id)
  {
    if (auto foundState = m_handlers.find(state);
        foundState != m_handlers.end())
    {
      return m_handlers[state]->RemoveBinding(id);
    }
    return false;
  }

  void StateInputHandler::HandleEvent(const SDL_Event& evnt)
  {
    if (m_handlers.find(m_currentState) != m_handlers.end())
    {
      m_handlers[m_currentState]->HandleEvent(evnt);
    }
    m_globalInputs.HandleEvent(evnt);
  }
  void StateInputHandler::Update()
  {
    if (m_handlers.find(m_currentState) != m_handlers.end())
    {
      m_handlers[m_currentState]->Update();
    }
    m_globalInputs.Update();
  }
} // namespace gk
