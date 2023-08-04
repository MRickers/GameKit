#include "StateManager.hpp"

gk::StateManager::StateManager(InputHandlerPtr input_handler)
    : m_input(std::move(input_handler))
{
}