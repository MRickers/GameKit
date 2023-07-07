#include "InputHandler.hpp"
#include "helpers/GameException.hpp"

#include <stdexcept>

bool gk::InputHandler::AddCallback(const std::string& id,
                                   EventCallback callback)
{
  std::exception_ptr eptr = nullptr;
  try
  {
    return m_callbacks.try_emplace(id, callback).second;
  }
  catch (...)
  {
    eptr = std::current_exception();
  }
  HandleException(eptr);
  return false;
}

bool gk::InputHandler::RemoveCallback(const std::string& id)
{
  std::exception_ptr eptr = nullptr;
  try
  {
    return m_callbacks.erase(id) == 1;
  }
  catch (...)
  {
    eptr = std::current_exception();
  }
  HandleException(eptr);
  return false;
}

bool gk::InputHandler::AddBinding(const std::string& id,
                                  const EventBinding binding)
{
  std::exception_ptr eptr = nullptr;
  try
  {
    return m_bindings.try_emplace(id, binding).second;
  }
  catch (...)
  {
    eptr = std::current_exception();
  }
  HandleException(eptr);
  return false;
}

bool gk::InputHandler::RemoveBinding(const std::string& id)
{
  std::exception_ptr eptr = nullptr;
  try
  {
    return m_bindings.erase(id) == 1;
  }
  catch (...)
  {
    eptr = std::current_exception();
  }
  HandleException(eptr);
  return false;
}

void gk::InputHandler::HandleEvent(const Event& evnt)
{
  switch (evnt.type)
  {
  case SDL_KEYDOWN:
    updateKeyStates();
    break;

  case SDL_KEYUP:
    updateKeyStates();
    break;

  default:
    break;
  }
}

void gk::InputHandler::Update()
{
  for (auto& [id, binding] : m_bindings)
  {
    for (auto& event : binding.events)
    {
      if (isKeyEvent(event.type))
      {
        if (keyDownEvent(event.type) && isKeyDown(event.key.keysym.scancode))
        {
          ++binding.event_counter;
        }
        else if (keyUpEvent(event.type) && isKeyUp(event.key.keysym.scancode))
        {
          ++binding.event_counter;
        }
      }
    }
    // invoke callback
    if (binding.event_counter == binding.events.size())
    {
      if (const auto callback = m_callbacks.find(binding.id);
          callback != m_callbacks.end())
      {
        callback->second(binding.event_details);
      }
    }
    binding.event_counter = 0;
    binding.event_details.Reset();
  }
}

bool gk::InputHandler::isKeyEvent(const uint32_t event_type)
{
  if (event_type == SDL_KEYDOWN || event_type == SDL_KEYUP)
  {
    return true;
  }
  return false;
}

void gk::InputHandler::updateKeyStates()
{
  m_keystates = SDL_GetKeyboardState(0);
}

bool gk::InputHandler::isKeyDown(SDL_Scancode key) const
{
  if (m_keystates != nullptr)
  {
    if (m_keystates[key] == 1)
    {
      return true;
    }
  }

  return false;
}

bool gk::InputHandler::isKeyUp(SDL_Scancode key) const
{
  return !isKeyDown(key);
}

bool gk::InputHandler::keyDownEvent(const uint32_t event_type) const
{
  return event_type == SDL_KEYDOWN;
}

bool gk::InputHandler::keyUpEvent(const uint32_t event_type) const
{
  return !keyDownEvent(event_type);
}
