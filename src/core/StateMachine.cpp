#include "StateMachine.hpp"

bool gk::StateMachine::hasState(const StateType state)
{
  return false;
}

void gk::StateMachine::registerState(const StateType state,
                                     StateCreator creator)
{
  if (m_factory.find(state) == m_factory.end())
  {
    m_factory[state] = creator;
  }
}
