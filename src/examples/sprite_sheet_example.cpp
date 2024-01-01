#include "GameKit/App.hpp"
#include "GameKit/core/sprite_sheet.hpp"
#include "GameKit/helpers/game_exception.hpp"
#include <spdlog/spdlog.h>

enum class StateType
{
  MAIN,
};

class game_state : public gk::base_state
{
public:
  explicit game_state(std::unique_ptr<gk::texture_manager>& t_texture_manager,
                      std::chrono::milliseconds const& t_update_rate)
      : m_sprite_sheet{t_texture_manager}
      , m_update_rate(t_update_rate)
  {
    if (!t_texture_manager->load_config("sprite_texture.cfg"))
    {
      spdlog::error("could not load sprite_texture.cfg");
    }
  }
  void on_create() override
  {

    if (!m_sprite_sheet.load_sheet({"sprite.sheet"}))
    {
      spdlog::info("could not load sprite.sheet");
    }
    else
    {
      spdlog::info("sprite.sheet loaded");
    }
  }
  void on_destroy() override
  {
  }
  void activate() override
  {
  }
  void deactivate() override
  {
  }
  void update() override
  {
    m_sprite_sheet.update(m_update_rate.count());
    m_sprite_sheet.set_sprite_position(m_pos);
    m_pos += {1,1};
  }
  void draw(SDL_Renderer* renderer) override
  {
    m_sprite_sheet.draw(renderer);
  }

private:
  gk::sprite_sheet m_sprite_sheet;
  gk::vector2d m_pos;
  std::chrono::milliseconds m_update_rate;
};

int main(int argc, const char** argv)
{
  try
  {
    gk::App app{{"Sprite test", 640, 480}};
    auto& input_handler = app.get_input_handler();

    {
      auto binding = gk::event_binding{"quit"};
      binding.events.push_back({gk::EventType::KeyDown, SDL_SCANCODE_Q});
      input_handler->add_global_binding(binding);
      input_handler->add_global_callback(
          "quit", [&](gk::event_details const& t_details) { app.stop(); });
    }

    auto& state_machine = app.get_state_machine();
    state_machine->register_state(
        StateType::MAIN, [&]() -> gk::base_state_ptr
        { return std::make_unique<game_state>(app.get_texture_manager(), app.get_update_rate()); });
    state_machine->switch_to(StateType::MAIN);
    app.run();
  }
  catch (gk::game_exception const& e)
  {

    std::cout << e.what();
  }
  gk::App::shutdown();
  return 0;
}