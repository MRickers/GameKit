#include "GameKit/core/StateMachine.hpp"
#include "GameKit/helpers/GameException.hpp"
#include <algorithm>

void gk::StateMachine::update()
{
  if (m_states.empty())
  {
    return;
  }

  if (m_states.back().second->isTranscendent() && m_states.size() > 1)
  {
    auto itr = m_states.end() - 1;
    while (itr != m_states.begin())
    {
      if (!itr->second->isTranscendent())
      {
        break;
      }
      --itr;
    }
    for (; itr != m_states.end(); ++itr)
    {
      itr->second->update();
    }
  }
  else
  {
    m_states.back().second->update();
  }
}

void gk::StateMachine::draw(SDL_Renderer* renderer)
{
  if (m_states.empty())
  {
    return;
  }

  if (m_states.back().second->isTransparent() && m_states.size() > 1)
  {
    auto itr = m_states.end() - 1;
    while (itr != m_states.begin())
    {
      if (!itr->second->isTransparent())
      {
        break;
      }
      --itr;
    }
    for (; itr != m_states.end(); ++itr)
    {
      itr->second->draw(renderer);
    }
  }
  else
  {
    m_states.back().second->draw(renderer);
  }
}

void gk::StateMachine::processRequests()
{
  while (m_toRemove.begin() != m_toRemove.end())
  {
    removeState(*m_toRemove.begin());
    m_toRemove.erase(m_toRemove.begin());
  }
}

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
      return true;
    }
  }
  return false;
}

void gk::StateMachine::switchTo(const StateType state)
{

  if (const auto toSwitch =
          std::find_if(m_states.begin(), m_states.end(),
                       [state](const std::pair<StateType, BaseStatePtr>& p)
                       { return state == p.first; });
      toSwitch != m_states.end())
  {
    m_states.back().second->deactivate();
    const auto tmpType = toSwitch->first;
    auto tmpState = std::move(toSwitch->second);
    auto* tmpStatePtr = tmpState.get();

    m_states.erase(toSwitch);
    m_states.emplace_back(tmpType, std::move(tmpState));
    tmpStatePtr->activate();
  }
  else
  {
    if (!m_states.empty())
    {
      m_states.back().second->deactivate();
    }
    createState(state);
    m_states.back().second->activate();

    m_currentState = state;
  }
}

void gk::StateMachine::remove(const StateType state)
{
  m_toRemove.push_back(state);
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

void gk::StateMachine::createState(const StateType state)
{
  if (const auto stateRegistered = m_factory.find(state);
      stateRegistered != m_factory.end())
  {
    auto newState = m_factory[state]();
    auto* newStatePtr = newState.get();

    m_states.emplace_back(state, std::move(newState));
    newStatePtr->onCreate();
    return;
  }
  throw gk::GameException(std::string{"could not create state: "} +
                              std::to_string(static_cast<int>(state)),
                          -100);
}

void gk::StateMachine::removeState(const StateType state)
{
  auto stateToRemove =
      std::find_if(m_states.begin(), m_states.end(),
                   [state](const std::pair<StateType, BaseStatePtr>& p)
                   { return state == p.first; });
  if (stateToRemove != m_states.end())
  {
    stateToRemove->second->onDestroy();
    m_states.erase(stateToRemove);
  }
}
