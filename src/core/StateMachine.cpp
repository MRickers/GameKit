#include "StateMachine.hpp"
#include <algorithm>

bool gk::StateMachine::hasState(const StateType state)
{
  const auto found =
      std::find_if(m_states.begin(), m_states.end(),
                   [state](const std::pair<StateType, BaseStatePtr>& p)
                   { return state == p.first; });
  if (found != m_states.end())
  {
    const auto removed = std::find_if(m_toRemove.begin(), m_toRemove.end(),
                                      [state](const StateType stateToRemove)
                                      { return state == stateToRemove; });
    if (removed == m_toRemove.end())
    {
      return false;
    }
  }
  return false;
}

void gk::StateMachine::registerState(const StateType state,
                                     StateCreator creator)
{
  m_factory[state] = creator;
}

StateType gk::StateMachine::currentState() const
{
  return m_currentState;
}
