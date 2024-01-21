#include "GameKit/core/input_handler.hpp"
#include "GameKit/helpers/game_exception.hpp"

#include <stdexcept>
static constexpr auto s_index_offset = 1;
//
// class gk::input_handler::details
//{
// public:
//  std::unordered_map<std::string, bool> m_invoked{};
//};
bool gk::input_handler::add_callback(const std::string& id,
                                     event_callback callback)
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
  handle_exception(eptr);
  return false;
}

bool gk::input_handler::remove_callback(const std::string& id)
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
  handle_exception(eptr);
  return false;
}

bool gk::input_handler::add_binding(const event_binding& binding)
{
  std::exception_ptr eptr = nullptr;
  try
  {
    if (binding.id.empty())
    {
      return false;
    }
    if (const auto added = m_details.try_emplace(binding.id, false).second;
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
  handle_exception(eptr);
  return false;
}

bool gk::input_handler::remove_binding(const std::string& id)
{
  std::exception_ptr eptr = nullptr;
  try
  {
    if (const auto removed = m_details.erase(id); removed != 1)
    {
      return false;
    }
    return m_bindings.erase(id) == 1;
  }
  catch (...)
  {
    eptr = std::current_exception();
  }
  handle_exception(eptr);
  return false;
}

/// @brief If no event happens, this method will not be invoked.
/// @param evnt
void gk::input_handler::handle_event(const SDL_Event& evnt)
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

void gk::input_handler::update()
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
          if (event.on_keyhold == event::on_keyhold_behaviour::invoke_repeat)
          {
            resetInvoked(binding.id);
          }
        }
        else
        {
          resetInvoked(binding.id);
        }
      }
      else if (isMouseButtonEvent(event.type))
      {
        if (isMouseButtonDown(event.mouse_button))
        {
          ++binding.event_counter;
          if (event.on_keyhold == event::on_keyhold_behaviour::invoke_repeat)
          {
            resetInvoked(binding.id);
          }
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
          binding.details.mouse_pos = {static_cast<float>(m_mouseX),
                                       static_cast<float>(m_mouseY)};
          callback->second(binding.details);
          setInvoked(binding.id);
        }
        binding.details.reset();
      }
    }
    binding.event_counter = 0;
  }
  m_mouseEvents[MouseButton::Motion - s_index_offset] = false;
}

bool gk::input_handler::isKeyEvent(const uint32_t event_type)
{
  return event_type == SDL_KEYDOWN || event_type == SDL_KEYUP;
}

bool gk::input_handler::isMouseButtonEvent(const uint32_t event_type)
{
  return event_type == SDL_MOUSEBUTTONDOWN || event_type == SDL_MOUSEBUTTONUP;
}

bool gk::input_handler::isMouseMotionEvent(const uint32_t event_type)
{
  return event_type == SDL_MOUSEMOTION;
}

void gk::input_handler::updateMouseStates(const uint32_t event_type)
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

void gk::input_handler::updateKeyStates()
{
  m_keystates = SDL_GetKeyboardState(NULL);
}

bool gk::input_handler::isKeyDown(SDL_Scancode key) const
{
  if (m_keystates != nullptr)
  {
    return m_keystates[key] == 1;
  }
  return false;
}

bool gk::input_handler::isKeyUp(SDL_Scancode key) const
{
  return !isKeyDown(key);
}

bool gk::input_handler::isMouseButtonDown(const MouseButton button) const
{
  return m_mouseEvents[button - s_index_offset];
}

bool gk::input_handler::isMotion() const
{
  return m_mouseEvents[MouseButton::Motion - s_index_offset];
}

bool gk::input_handler::setInvoked(std::string const& id)
{
  if (const auto invoked = m_details.find(id); invoked != m_details.end())
  {
    invoked->second = true;
    return true;
  }
  return false;
}

bool gk::input_handler::resetInvoked(std::string const& id)
{
  if (const auto invoked = m_details.find(id); invoked != m_details.end())
  {
    invoked->second = false;
    return true;
  }
  return false;
}

bool gk::input_handler::wasInvoked(const std::string& id) const
{
  if (const auto invoked = m_details.find(id); invoked != m_details.end())
  {
    return invoked->second;
  }
  return false;
}
gk::event_binding gk::make_keydown_binding(std::string_view const& t_id,
                                           SDL_Scancode t_scancode)
{
  auto binding = gk::event_binding{std::string{t_id}};
  binding.events.push_back({gk::EventType::KeyDown,
                            gk::event::on_keyhold_behaviour::invoke_once,
                            t_scancode});

  return binding;
}
gk::event_binding gk::make_keydown_repeat_binding(std::string_view const& t_id,
                                                  SDL_Scancode t_scancode)
{
  auto binding = gk::event_binding{std::string{t_id}};
  binding.events.push_back({gk::EventType::KeyDown,
                            gk::event::on_keyhold_behaviour::invoke_repeat,
                            t_scancode});

  return binding;
}
