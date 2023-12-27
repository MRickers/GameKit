#pragma once
#include "GameKit/core/input_handler.hpp"
#include <unordered_map>

enum class StateType;

namespace gk
{

  class StateInputHandler
  {
    using InputHandlerOwner = std::unique_ptr<input_handler>;
    using InputHandlers = std::unordered_map<StateType, InputHandlerOwner>;

  public:
    void setCurrentState(const StateType state);
    StateType currentState() const;

    bool AddGlobalCallback(const std::string& id, event_callback);
    bool RemoveGlobalCallback(const std::string& id);

    bool AddGlobalBinding(const event_binding&);
    bool RemoveGlobalBinding(const std::string& id);

    bool add_callback(const StateType state, const std::string& id,
                      event_callback);
    bool remove_callback(const StateType state, const std::string& id);

    bool add_binding(const StateType state, const event_binding&);
    bool remove_binding(const StateType state, const std::string& id);

    void handle_event(const SDL_Event&);
    void Update();

  private:
    InputHandlers m_handlers;
    input_handler m_globalInputs;
    StateType m_currentState;
  };

} // namespace gk
