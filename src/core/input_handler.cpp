#include "GameKit/core/InputHandler.hpp"
#include "GameKit/helpers/GameException.hpp"

#include <array>
#include <stdexcept>
#include <unordered_map>
#include <vector>

static constexpr auto s_index_offset = 1;
namespace gk
{

  // Index 0 = left mouse button pressed
  // Index 1 = middle mouse button pressed
  // Index 2 = right mouse button pressed
  // Index 4 = motion
  using MouseButtonState = std::array<bool, MouseButton::Motion>;

  enum EventType
  {
    Idle,
    KeyDown = SDL_KEYDOWN,
    KeyUp = SDL_KEYUP,
    MouseMotion = SDL_MOUSEMOTION,
    MouseDown = SDL_MOUSEBUTTONDOWN,
    MouseUp = SDL_MOUSEBUTTONUP,

  };

  struct event
  {
    enum class on_keyhold_behaviour
    {
      invoke_once,
      invoke_repeat,
    };
    EventType type{Idle};
    // only relevant for keydown events. If multiple keys are used and one is
    // set to "invoke_repeat" this behaviour is used for that binding
    on_keyhold_behaviour on_keyhold{on_keyhold_behaviour::invoke_once};
    SDL_Scancode scancode{SDL_SCANCODE_UNKNOWN};
    MouseButton mouse_button{Left};
  };

  using event_array = std::vector<event>;

  struct event_binding
  {
    explicit event_binding(std::string const& t_id)
        : id{t_id}
    {
    }

    std::string id;
    event_array events{};
    size_t event_counter{0};
    event_details details{};
    bool already_invoked{false};
  };

  using event_callbacks = std::unordered_map<std::string, event_callback>;
  using event_bindings = std::unordered_map<std::string, event_binding>;

  struct InputHandler::details
  {
    event_callbacks callbacks{};
    event_bindings bindings{};

    std::unordered_map<std::string, bool> bindings_invoked{};

    const Uint8* keystates{nullptr};
    MouseButtonState mouseEvents{};
    bool mouseMotion{false};
    int mouseX{0};
    int mouseY{0};

    static bool isKeyEvent(uint32_t event_type);
    static bool isMouseButtonEvent(uint32_t event_type);
    static bool isMouseMotionEvent(uint32_t event_type);
    bool add_binding(event_binding const&);
    void updateMouseStates(uint32_t event_type);
    void updateKeyStates();
    bool isKeyDown(SDL_Scancode key) const;
    bool isKeyUp(SDL_Scancode key) const;
    bool isMouseButtonDown(MouseButton button) const;
    bool isMotion() const;

    bool setInvoked(std::string const& id);
    bool resetInvoked(std::string const& id);
    bool wasInvoked(const std::string& id) const;
  };

} // namespace gk

gk::InputHandler::InputHandler()
    : m_details{std::make_unique<gk::InputHandler::details>()}
{
}

bool gk::InputHandler::add_callback(const std::string& id,
                                     event_callback callback)
{
  std::exception_ptr exception_ptr = nullptr;
  try
  {
    return m_details->callbacks.try_emplace(id, callback).second;
  }
  catch (...)
  {
    exception_ptr = std::current_exception();
  }
  handle_exception(exception_ptr);
  return false;
}

bool gk::InputHandler::remove_callback(const std::string& id)
{
  std::exception_ptr exception_ptr = nullptr;
  try
  {
    return m_details->callbacks.erase(id) == 1;
  }
  catch (...)
  {
    exception_ptr = std::current_exception();
  }
  handle_exception(exception_ptr);
  return false;
}

bool gk::InputHandler::remove_binding(const std::string& id)
{
  std::exception_ptr exception_ptr = nullptr;
  try
  {
    if (const auto removed = m_details->bindings_invoked.erase(id);
        removed != 1)
    {
      return false;
    }
    return m_details->bindings.erase(id) == 1;
  }
  catch (...)
  {
    exception_ptr = std::current_exception();
  }
  handle_exception(exception_ptr);
  return false;
}

/// @brief If no event happens, this method will not be invoked.
/// @param evnt
void gk::InputHandler::handle_event(const SDL_Event& evnt)
{
  switch (evnt.type)
  {
  case SDL_KEYDOWN:
  case SDL_KEYUP:
    m_details->updateKeyStates();
    break;
  case SDL_MOUSEMOTION:
  case SDL_MOUSEBUTTONDOWN:
  case SDL_MOUSEBUTTONUP:
    m_details->updateMouseStates(evnt.type);
    break;
  default:
    break;
  }
}

void gk::InputHandler::update()
{
  for (auto& [id, binding] : m_details->bindings)
  {
    for (auto& event : binding.events)
    {
      if (m_details->isKeyEvent(event.type))
      {
        if (m_details->isKeyDown(event.scancode))
        {
          ++binding.event_counter;
          if (event.on_keyhold == event::on_keyhold_behaviour::invoke_repeat)
          {
            m_details->resetInvoked(binding.id);
          }
        }
        else
        {
          m_details->resetInvoked(binding.id);
        }
      }
      else if (m_details->isMouseButtonEvent(event.type))
      {
        if (m_details->isMouseButtonDown(event.mouse_button))
        {
          ++binding.event_counter;
          if (event.on_keyhold == event::on_keyhold_behaviour::invoke_repeat)
          {
            m_details->resetInvoked(binding.id);
          }
        }
        else
        {
          m_details->resetInvoked(binding.id);
        }
      }
      else if (m_details->isMouseMotionEvent(event.type))
      {
        if (m_details->isMotion())
        {
          m_details->resetInvoked(binding.id);
          ++binding.event_counter;
        }
      }
    }
    // invoke callback
    if (binding.event_counter == binding.events.size())
    {
      if (const auto callback = m_details->callbacks.find(binding.id);
          callback != m_details->callbacks.end())
      {
        if (!m_details->wasInvoked(binding.id))
        {
          binding.details.mouse_pos = {static_cast<float>(m_details->mouseX),
                                       static_cast<float>(m_details->mouseY)};
          callback->second(binding.details);
          m_details->setInvoked(binding.id);
        }
        binding.details.reset();
      }
    }
    binding.event_counter = 0;
  }
  m_details->mouseEvents[MouseButton::Motion - s_index_offset] = false;
}

bool gk::InputHandler::details::add_binding(
    event_binding const& t_event_binding)
{
  std::exception_ptr exception_ptr = nullptr;
  try
  {
    if (t_event_binding.id.empty())
    {
      return false;
    }
    if (const auto added =
            bindings_invoked.try_emplace(t_event_binding.id, false).second;
        !added)
    {
      return false;
    }
    return bindings.try_emplace(t_event_binding.id, t_event_binding).second;
  }
  catch (...)
  {
    exception_ptr = std::current_exception();
  }
  handle_exception(exception_ptr);
  return false;
}

bool gk::InputHandler::add_keydown_binding(std::string_view id,
                                            gk::keys const& t_keys,
                                            mouse_buttons t_mouse_buttons)
{
  auto binding = gk::event_binding{std::string{id}};
  for (auto const& key : t_keys)
  {
    binding.events.push_back({gk::EventType::KeyDown,
                              gk::event::on_keyhold_behaviour::invoke_once,
                              key});
  }

  for(auto const & button : t_mouse_buttons) {
    binding.events.push_back({gk::EventType::MouseDown,
                              gk::event::on_keyhold_behaviour::invoke_once,
                              SDL_SCANCODE_UNKNOWN, button});
  }
  return m_details->add_binding(binding);
}

bool gk::InputHandler::add_keydown_repeat_binding(
    std::string_view id, keys const& t_keys, mouse_buttons t_mouse_buttons)
{
  auto binding = gk::event_binding{std::string{id}};
  for (auto const& key : t_keys)
  {
    binding.events.push_back({gk::EventType::KeyDown,
                              gk::event::on_keyhold_behaviour::invoke_repeat,
                              key});
  }
  for(auto const& button : t_mouse_buttons)
  {
    binding.events.push_back({gk::EventType::MouseDown,
                              gk::event::on_keyhold_behaviour::invoke_repeat,
                              SDL_SCANCODE_UNKNOWN, button});
  }
  return m_details->add_binding(binding);
}
gk::InputHandler::~InputHandler() = default;

// input_handler::details
bool gk::InputHandler::details::isKeyEvent(const uint32_t event_type)
{
  return event_type == SDL_KEYDOWN || event_type == SDL_KEYUP;
}

bool gk::InputHandler::details::isMouseButtonEvent(const uint32_t event_type)
{
  return event_type == SDL_MOUSEBUTTONDOWN || event_type == SDL_MOUSEBUTTONUP;
}

bool gk::InputHandler::details::isMouseMotionEvent(const uint32_t event_type)
{
  return event_type == SDL_MOUSEMOTION;
}

void gk::InputHandler::details::updateMouseStates(const uint32_t event_type)
{
  const auto mouseButtons = SDL_GetMouseState(&mouseX, &mouseY);

  if (mouseButtons & SDL_BUTTON(gk::MouseButton::Left))
  {
    mouseEvents[gk::MouseButton::Left - s_index_offset] = true;
  }
  else
  {
    mouseEvents[gk::MouseButton::Left - s_index_offset] = false;
  }
  if (mouseButtons & SDL_BUTTON(gk::MouseButton::Middle))
  {
    mouseEvents[gk::MouseButton::Middle - s_index_offset] = true;
  }
  else
  {
    mouseEvents[gk::MouseButton::Middle - s_index_offset] = false;
  }
  if (mouseButtons & SDL_BUTTON(gk::MouseButton::Right))
  {
    mouseEvents[gk::MouseButton::Right - s_index_offset] = true;
  }
  else
  {
    mouseEvents[gk::MouseButton::Right - s_index_offset] = false;
  }
  if (event_type == SDL_MOUSEMOTION)
  {
    mouseEvents[gk::MouseButton::Motion - s_index_offset] = true;
  }
}

void gk::InputHandler::details::updateKeyStates()
{
  keystates = SDL_GetKeyboardState(nullptr);
}

bool gk::InputHandler::details::isKeyDown(SDL_Scancode key) const
{
  if (keystates != nullptr)
  {
    return keystates[key] == 1;
  }
  return false;
}

bool gk::InputHandler::details::isKeyUp(SDL_Scancode key) const
{
  return !isKeyDown(key);
}

bool gk::InputHandler::details::isMouseButtonDown(
    const MouseButton button) const
{
  return mouseEvents[button - s_index_offset];
}

bool gk::InputHandler::details::isMotion() const
{
  return mouseEvents[MouseButton::Motion - s_index_offset];
}

bool gk::InputHandler::details::setInvoked(std::string const& id)
{
  if (const auto invoked = bindings_invoked.find(id);
      invoked != bindings_invoked.end())
  {
    invoked->second = true;
    return true;
  }
  return false;
}

bool gk::InputHandler::details::resetInvoked(std::string const& id)
{
  if (const auto invoked = bindings_invoked.find(id);
      invoked != bindings_invoked.end())
  {
    invoked->second = false;
    return true;
  }
  return false;
}

bool gk::InputHandler::details::wasInvoked(const std::string& id) const
{
  if (const auto invoked = bindings_invoked.find(id);
      invoked != bindings_invoked.end())
  {
    return invoked->second;
  }
  return false;
}