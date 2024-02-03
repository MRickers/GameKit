#pragma once
#include <functional>
#include <memory>
#include <vector>

#include "GameKit/core/BaseState.hpp"

enum class StateType;
class SDL_Renderer;

namespace gk
{
  using BaseStatePtr = std::unique_ptr<BaseState>;
  using StateCreator = std::function<BaseStatePtr()>;

  class StateMachine
  {
    using StateContainer = std::vector<std::pair<StateType, BaseStatePtr>>;
    using TypeContainer = std::vector<StateType>;
    using StateFactory = std::unordered_map<StateType, StateCreator>;

  public:
    void update();
    void draw(SDL_Renderer*);
    void process_requests();
    bool has_state(const StateType state);
    void switch_to(const StateType state);
    void remove(const StateType state);
    void register_state(const StateType state, StateCreator creator);
    StateType current_state() const;

  private:
    StateType m_current_state{};
    StateContainer m_states;
    TypeContainer m_toRemove;
    StateFactory m_factory;

    void create_state(const StateType state);
    void remove_state(const StateType state);
  };

} // namespace gk
