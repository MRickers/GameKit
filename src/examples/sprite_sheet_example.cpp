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
  enum class state_t
  {
    IDLE,
    WALKING,
    JUMP,
  };

public:
  explicit game_state(std::unique_ptr<gk::texture_manager>& t_texture_manager,
                      std::chrono::milliseconds const& t_update_rate,
                      std::unique_ptr<gk::state_input_handler>& t_input_handler)
      : m_sprite_sheet{t_texture_manager}
      , m_update_rate(t_update_rate)
  {
    if (!t_texture_manager->load_config("sprite_texture.cfg"))
    {
      spdlog::error("could not load sprite_texture.cfg");
    }
    {
      t_input_handler->add_keydown_repeat_binding(StateType::MAIN, "left", {SDL_SCANCODE_A});
      t_input_handler->add_callback(StateType::MAIN, "left",
                                    [this](gk::event_details const&)
                                    {
                                      m_pos += {-1, 0};
                                      m_sprite_sheet.set_direction(
                                          gk::sprite_sheet::direction::LEFT);
                                      m_state = state_t::WALKING;
                                    });
    }
    {
      t_input_handler->add_keydown_repeat_binding(StateType::MAIN, "right", {SDL_SCANCODE_D});
      t_input_handler->add_callback(StateType::MAIN, "right",
                                    [this](gk::event_details const&)
                                    {
                                      m_pos += {1, 0};
                                      m_sprite_sheet.set_direction(
                                          gk::sprite_sheet::direction::RIGHT);
                                      m_state = state_t::WALKING;
                                    });
    }
  }
  void on_create() override
  {
    if (!m_sprite_sheet.load_sheet({"sprite.sheet"}))
    {
      spdlog::info("could not load sprite.sheet");
    }
    m_sprite_sheet.get_current_animation()->set_looping(true);
    m_sprite_sheet.set_direction(gk::sprite_sheet::direction::RIGHT);
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
    m_sprite_sheet.set_sprite_position(m_pos);
    if (m_state == state_t::WALKING)
    {
      m_sprite_sheet.set_animation("Walk", true, true);
    }
    else
    {
      m_sprite_sheet.set_animation("Idle", true, true);
    }
    m_sprite_sheet.update(m_update_rate.count());
  }
  void draw(SDL_Renderer* renderer) override
  {
    m_sprite_sheet.draw(renderer);
    m_state = state_t::IDLE;
  }

private:
  gk::sprite_sheet m_sprite_sheet;
  gk::vector2d m_pos{300, 200};
  std::chrono::milliseconds m_update_rate;
  gk::vector2d m_vel{0, 0};
  state_t m_state{state_t::IDLE};
};

int main(int argc, const char** argv)
{
  try
  {
    gk::App app{{"Sprite test", 640, 480}};
    auto& input_handler = app.get_input_handler();

    {
      input_handler->add_global_keydown_binding("quit", {SDL_SCANCODE_Q});
      input_handler->add_global_callback(
          "quit", [&](gk::event_details const& t_details) { app.stop(); });
    }

    auto& state_machine = app.get_state_machine();
    state_machine->register_state(StateType::MAIN,
                                  [&]() -> gk::base_state_ptr
                                  {
                                    return std::make_unique<game_state>(
                                        app.get_texture_manager(),
                                        app.get_update_rate(),
                                        app.get_input_handler());
                                  });
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