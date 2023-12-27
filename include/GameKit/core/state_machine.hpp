#pragma once
#include <functional>
#include <memory>
#include <vector>

#include "GameKit/core/base_state.hpp"

enum class StateType;
class SDL_Renderer;

namespace gk
{
  using base_state_ptr = std::unique_ptr<base_state>;
  using state_creator = std::function<base_state_ptr()>;

  class state_machine
  {
    using state_container = std::vector<std::pair<StateType, base_state_ptr>>;
    using type_container = std::vector<StateType>;
    using state_factory = std::unordered_map<StateType, state_creator>;

  public:
    void update();
    void draw(SDL_Renderer*);
    void processRequests();
    bool hasState(const StateType state);
    void switchTo(const StateType state);
    void remove(const StateType state);
    void registerState(const StateType state, state_creator creator);
    StateType current_state() const;

  private:
    StateType m_current_state{};
    state_container m_states;
    type_container m_toRemove;
    state_factory m_factory;

    void createState(const StateType state);
    void removeState(const StateType state);
  };

} // namespace gk
