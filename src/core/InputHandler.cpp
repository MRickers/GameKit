#include "InputHandler.hpp"
#include "helpers/GameException.hpp"

#include <iostream>
#include <stdexcept>
static constexpr auto s_index_offset = 1;
class gk::InputHandler::Details
{
public:
  std::unordered_map<std::string, bool> m_invoked{};
};

gk::InputHandler::InputHandler()
    : m_details{std::make_unique<InputHandler::Details>()}
{
}

gk::InputHandler::~InputHandler()
{
}

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
    if (const auto added =
            m_details->m_invoked.try_emplace(binding.id, false).second;
        !added)
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
    if (const auto removed = m_details->m_invoked.erase(id); removed != 1)
    {
      return false;
    }
    return m_bindings.erase(id) == 1;
  }
  catch (...)
  {
    eptr = std::current_exception();
  }
  HandleException(eptr);
  return false;
}

/// @brief If no event happens, this method will not be invoked.
/// @param evnt
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
    updateMouseStates(evnt.type);
    break;

  case SDL_MOUSEBUTTONDOWN:
    updateMouseStates(evnt.type);
    break;

  case SDL_MOUSEBUTTONUP:
    updateMouseStates(evnt.type);
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
        else
        {
          resetInvoked(binding.id);
        }
      }
      else if (isMouseButtonEvent(event.type))
      {
        if (isMouseButtonDown(event.mouseButton))
        {
          ++binding.event_counter;
        }
        else
        {
          resetInvoked(binding.id);
        }
      }
      else if (isMouseMotionEvent(event.type))
      {
        if (isMotion())
        {
          resetInvoked(binding.id);
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
        if (!wasInvoked(binding.id))
        {
          binding.event_details.mouse_pos = {static_cast<float>(m_mouseX),
                                             static_cast<float>(m_mouseY)};
          callback->second(binding.event_details);
          setInvoked(binding.id);
        }
        binding.event_details.Reset();
      }
    }
    binding.event_counter = 0;
  }
  m_mouseEvents[MouseButton::Motion - s_index_offset] = false;
}

bool gk::InputHandler::isKeyEvent(const uint32_t event_type)
{
  return event_type == SDL_KEYDOWN || event_type == SDL_KEYUP;
}

bool gk::InputHandler::isMouseButtonEvent(const uint32_t event_type)
{
  return event_type == SDL_MOUSEBUTTONDOWN || event_type == SDL_MOUSEBUTTONUP;
}

bool gk::InputHandler::isMouseMotionEvent(const uint32_t event_type)
{
  return event_type == SDL_MOUSEMOTION;
}

void gk::InputHandler::updateMouseStates(const uint32_t event_type)
{
  const auto mouseButtons = SDL_GetMouseState(&m_mouseX, &m_mouseY);

  if (mouseButtons & SDL_BUTTON(gk::MouseButton::Left))
  {
    m_mouseEvents[gk::MouseButton::Left - s_index_offset] = true;
  }
  else
  {
    m_mouseEvents[gk::MouseButton::Left - s_index_offset] = false;
  }
  if (mouseButtons & SDL_BUTTON(gk::MouseButton::Middle))
  {
    m_mouseEvents[gk::MouseButton::Middle - s_index_offset] = true;
  }
  else
  {
    m_mouseEvents[gk::MouseButton::Middle - s_index_offset] = false;
  }
  if (mouseButtons & SDL_BUTTON(gk::MouseButton::Right))
  {
    m_mouseEvents[gk::MouseButton::Right - s_index_offset] = true;
  }
  else
  {
    m_mouseEvents[gk::MouseButton::Right - s_index_offset] = false;
  }
  if (event_type == SDL_MOUSEMOTION)
  {
    m_mouseEvents[gk::MouseButton::Motion - s_index_offset] = true;
  }
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
  return m_mouseEvents[button - s_index_offset];
}

bool gk::InputHandler::isMotion() const
{
  return m_mouseEvents[MouseButton::Motion - s_index_offset];
}

bool gk::InputHandler::setInvoked(const std::string id)
{
  if (const auto invoked = m_details->m_invoked.find(id);
      invoked != m_details->m_invoked.end())
  {
    invoked->second = true;
    return true;
  }
  return false;
}

bool gk::InputHandler::resetInvoked(const std::string id)
{
  if (const auto invoked = m_details->m_invoked.find(id);
      invoked != m_details->m_invoked.end())
  {
    invoked->second = false;
    return true;
  }
  return false;
}

bool gk::InputHandler::wasInvoked(const std::string& id) const
{
  if (const auto invoked = m_details->m_invoked.find(id);
      invoked != m_details->m_invoked.end())
  {
    return invoked->second;
  }
  return false;
}
