#pragma once
#include "InputHandler.hpp"
#include <unordered_map>

enum class StateType;

namespace gk
{

  class StateInputHandler
  {
    using InputHandlerOwner = std::unique_ptr<InputHandler>;
    using InputHandlers = std::unordered_map<StateType, InputHandlerOwner>;

  public:
    void setCurrentState(const StateType state);
    StateType currentState() const;

    bool AddGlobalCallback(const std::string& id, EventCallback);
    bool RemoveGlobalCallback(const std::string& id);

    bool AddGlobalBinding(const EventBinding&);
    bool RemoveGlobalBinding(const std::string& id);

    bool AddCallback(const StateType state, const std::string& id,
                     EventCallback);
    bool RemoveCallback(const StateType state, const std::string& id);

    bool AddBinding(const StateType state, const EventBinding&);
    bool RemoveBinding(const StateType state, const std::string& id);

    void HandleEvent(const SDL_Event&);
    void Update();

  private:
    InputHandlers m_handlers;
    InputHandler m_globalInputs;
    StateType m_currentState;
  };

} // namespace gk
