#pragma once
#include <functional>
#include <memory>
#include <vector>

#include "BaseState.hpp"

enum class StateType;
class SDL_Renderer;

namespace gk
{
  using BaseStatePtr = std::unique_ptr<IBaseState>;
  using StateCreator = std::function<BaseStatePtr()>;

  class StateMachine
  {
    using StateContainer = std::vector<std::pair<StateType, BaseStatePtr>>;
    using TypeContainer = std::vector<StateType>;
    using StateFactory = std::unordered_map<StateType, StateCreator>;

  public:
    void update();
    void draw(const SDL_Renderer*);
    void processRequests();
    bool hasState(const StateType state);
    void switchTo(const StateType state);
    void remove(const StateType state);
    void registerState(const StateType state, StateCreator creator);
    StateType currentState() const;

  private:
    StateType m_currentState{};
    StateContainer m_states;
    TypeContainer m_toRemove;
    StateFactory m_factory;

    void createState(const StateType state);
    void removeState(const StateType state);
  };

} // namespace gk
