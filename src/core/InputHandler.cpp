#include "InputHandler.hpp"
#include "helpers/GameException.hpp"

#include <iostream>
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

bool gk::InputHandler::AddBinding(const EventBinding& binding)
{
  std::exception_ptr eptr = nullptr;
  try
  {
    if (binding.id.empty())
    {
      return false;
    }
    return m_bindings.try_emplace(binding.id, binding).second;
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

void gk::InputHandler::HandleEvent(const SDL_Event& evnt)
{
  switch (evnt.type)
  {
  case SDL_KEYDOWN:
    updateKeyStates();
    break;

  case SDL_KEYUP:
    updateKeyStates();
    break;

  case SDL_MOUSEMOTION:
    updateMouseStates();
    break;

  case SDL_MOUSEBUTTONDOWN:
    updateMouseStates();
    break;

  case SDL_MOUSEBUTTONUP:
    updateMouseStates();
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
        if (isKeyDown(event.scancode))
        {
          ++binding.event_counter;
        }
        else if (isKeyUp(event.scancode))
        {
          binding.event_counter = 0;
          binding.already_invoked = false;
        }
      }
      else if (isMouseEvent(event.type))
      {
        if (isMouseButtonDown(event.mouseButton))
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
        if (!binding.already_invoked)
        {
          callback->second(binding.event_details);
          binding.already_invoked = true;
        }
      }
    }
    binding.event_details.Reset();
  }
}

bool gk::InputHandler::isKeyEvent(const uint32_t event_type)
{
  return event_type == SDL_KEYDOWN || event_type == SDL_KEYUP;
}

bool gk::InputHandler::isMouseEvent(const uint32_t event_type)
{
  return event_type == SDL_MOUSEMOTION || event_type == SDL_MOUSEBUTTONDOWN ||
         event_type == SDL_MOUSEBUTTONUP;
}

void gk::InputHandler::updateMouseStates()
{
  static constexpr auto index_offset = 1;
  m_mouseEvents[SDL_GetMouseState(&m_mouseX, &m_mouseY) - index_offset] = true;
}

void gk::InputHandler::updateKeyStates()
{
  m_keystates = SDL_GetKeyboardState(NULL);
}

bool gk::InputHandler::isKeyDown(SDL_Scancode key) const
{
  if (m_keystates != nullptr)
  {
    return m_keystates[key] == 1;
  }
  return false;
}

bool gk::InputHandler::isKeyUp(SDL_Scancode key) const
{
  return !isKeyDown(key);
}

bool gk::InputHandler::isMouseButtonDown(const MouseButton button) const
{
  return m_mouseEvents[button];
}
