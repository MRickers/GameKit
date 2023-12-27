#include "GameKit/App.hpp"
#include "GameKit/core/ui/text_box.hpp"
#include "GameKit/helpers/Draw.hpp"
#include "GameKit/helpers/Timer.hpp"
#include <spdlog/spdlog.h>

enum class StateType
{
  INTRO,
  MAIN,
  GAME,
  PAUSED,
};

class IntroState : public gk::base_state
{
public:
  IntroState(gk::SharedContextPtr sharedContext, const gk::vector2d& windowSize)
      : m_sharedContext{sharedContext}
      , m_windowSize{windowSize}
  {
  }

  void on_create() override
  {
    m_pos = m_windowSize / 2 - m_size / 2;

    m_sharedContext->inputHandler->add_callback(
        StateType::INTRO, "IntroContinue",
        [this](const gk::event_details& details) { changeState(details); });

    auto binding = gk::event_binding{"IntroContinue"};
    binding.events.push_back({gk::EventType::KeyDown, SDL_SCANCODE_SPACE});
    m_sharedContext->inputHandler->add_binding(StateType::INTRO, binding);
  }
  void on_destroy() override
  {
    m_sharedContext->inputHandler->remove_binding(StateType::INTRO,
                                                  "IntroContinue");
    m_sharedContext->inputHandler->remove_callback(StateType::INTRO,
                                                   "IntroContinue");
  }

  void activate() override
  {
    m_timer.Start();
  }
  void deactivate() override
  {
  }

  void update() override
  {
    if (!m_timer.HasPassed(2000))
    {
      m_pos += gk::vector2d{0, 1};
    }
  }
  void draw(SDL_Renderer* renderer) override
  {
    gk::Draw::setRendererColor(renderer, gk::Color::MAROON);
    gk::Draw::filledRect(renderer, m_pos, m_size);
  }

private:
  void changeState(const gk::event_details&)
  {
    if (m_timer.HasPassed(2000))
    {
      if (m_sharedContext)
      {
        m_sharedContext->state_machine->switchTo(StateType::MAIN);
        m_sharedContext->state_machine->remove(StateType::INTRO);
        m_sharedContext->inputHandler->set_current_state(StateType::MAIN);
      }
    }
  }

  gk::SharedContextPtr m_sharedContext{nullptr};
  gk::Timer m_timer;
  gk::vector2d m_windowSize{0, 0};
  gk::vector2d m_pos{0, 0};
  gk::vector2d m_size{200, 100};
};

class MainState : public gk::base_state
{
  struct Shape
  {
    gk::vector2d m_pos{0, 0};
    gk::vector2d m_size{0, 0};
  };

public:
  MainState(gk::SharedContextPtr sharedContext)
      : m_sharedContext{sharedContext}
  {
  }

  void on_create() override
  {
    const auto [x, y] = m_buttonPos.Get();
    for (int i = 0; i < 3; ++i)
    {
      const auto buttonPos = gk::vector2d{
          x, y + (i * (m_buttonSize.GetY<int>() + m_buttonPadding))};

      m_shapes[i].m_pos = buttonPos + m_buttonSize / 2;
      m_shapes[i].m_size = m_buttonSize;
    }
    m_sharedContext->inputHandler->add_callback(
        StateType::MAIN, "MouseLeft",
        [this](const gk::event_details& details) { mouseClick(details); });

    auto binding = gk::event_binding{"MouseLeft"};
    binding.events.push_back({gk::EventType::MouseDown, SDL_SCANCODE_UNKNOWN,
                              gk::MouseButton::Left});
    m_sharedContext->inputHandler->add_binding(StateType::MAIN, binding);
  }
  void on_destroy() override
  {
    m_sharedContext->inputHandler->remove_binding(StateType::INTRO,
                                                  "IntroContinue");
    m_sharedContext->inputHandler->remove_callback(StateType::INTRO,
                                                   "IntroContinue");
  }

  void activate() override
  {
  }
  void deactivate() override
  {
  }

  void update() override
  {
  }
  void draw(SDL_Renderer* renderer) override
  {
    gk::Draw::setRendererColor(renderer, gk::Color::LIME);
    for (int i = 0; i < 3; ++i)
    {
      gk::Draw::filledRect(renderer, m_shapes[i].m_pos, m_shapes[i].m_size);
    }
  }

private:
  void mouseClick(const gk::event_details& details)
  {
    const auto [mouseX, mouseY] = details.mouse_pos.Get();
    const auto [btnSizeX, btnSizeY] = m_buttonSize.Get();

    for (int i = 0; i < 3; ++i)
    {
      if (mouseX >= m_shapes[i].m_pos.GetX<int>() &&
          mouseX <= m_shapes[i].m_pos.GetX<int>() + btnSizeX &&
          mouseY >= m_shapes[i].m_pos.GetY<int>() &&
          mouseY <= m_shapes[i].m_pos.GetY<int>() + btnSizeY)
      {
        if (i == 0)
        {
          m_sharedContext->state_machine->switchTo(StateType::GAME);
          m_sharedContext->inputHandler->set_current_state(StateType::GAME);
        }
        else if (i == 1)
        {
          spdlog::info("Options");
        }
        else
        {
          m_sharedContext->app->stop();
        }
      }
    }
  }

  gk::SharedContextPtr m_sharedContext{nullptr};

  gk::vector2d m_buttonSize{100, 50};
  gk::vector2d m_buttonPos{220, 90};
  uint32_t m_buttonPadding{10};
  Shape m_shapes[3];
};

class GameState : public gk::base_state
{
  struct Shape
  {
    gk::vector2d m_pos{0, 0};
    gk::vector2d m_size{0, 0};
  };

public:
  GameState(gk::SharedContextPtr sharedContext)
      : m_sharedContext{sharedContext}
  {
  }

  void on_create() override
  {
    m_shape.m_pos = {320, 230};
    m_shape.m_size = {20, 0};

    {
      m_sharedContext->inputHandler->add_callback(
          StateType::GAME, "MouseMotion",
          [this](const gk::event_details& details) { mouseMotion(details); });

      auto binding = gk::event_binding{"MouseMotion"};
      binding.events.push_back({gk::EventType::MouseMotion,
                                SDL_SCANCODE_UNKNOWN, gk::MouseButton::Motion});
      m_sharedContext->inputHandler->add_binding(StateType::GAME, binding);
    }
    {
      m_sharedContext->inputHandler->add_callback(
          StateType::GAME, "Pause",
          [this](const gk::event_details& details) { paused(details); });

      auto binding = gk::event_binding{"Pause"};
      binding.events.push_back({gk::EventType::KeyDown, SDL_SCANCODE_P});
      m_sharedContext->inputHandler->add_binding(StateType::GAME, binding);
    }
    {
      m_sharedContext->inputHandler->add_callback(
          StateType::GAME, "Quit",
          [this](const gk::event_details& details)
          { m_sharedContext->app->stop(); });

      auto binding = gk::event_binding{"Quit"};
      binding.events.push_back({gk::EventType::KeyDown, SDL_SCANCODE_Q});
      m_sharedContext->inputHandler->add_binding(StateType::GAME, binding);
    }

    if (m_font = TTF_OpenFont("Roboto-Regular.ttf", 10); m_font == nullptr)
    {
      spdlog::warn("could not load font: ", TTF_GetError());
    }
    m_textBox.setFont(m_font, 12);
    m_textBox.add("created main state");
    m_textBox.setPos({4, 4});
  }
  void on_destroy() override
  {
    m_sharedContext->inputHandler->remove_binding(StateType::GAME,
                                                  "MouseMotion");
    m_sharedContext->inputHandler->remove_callback(StateType::GAME,
                                                   "MouseMotion");
    m_sharedContext->inputHandler->remove_binding(StateType::GAME, "Pause");
    m_sharedContext->inputHandler->remove_callback(StateType::GAME, "Pause");
    m_sharedContext->inputHandler->remove_binding(StateType::GAME, "Quit");
    m_sharedContext->inputHandler->remove_callback(StateType::GAME, "Quit");
    TTF_CloseFont(m_font);
  }

  void activate() override
  {
    m_textBox.add("activating game state");
  }
  void deactivate() override
  {
    m_textBox.add("deactivating game state");
  }

  void update() override
  {
    auto acc = m_mousePos - m_shape.m_pos;
    acc.SetLimit(2);

    m_vel += acc;

    m_vel.SetLimit(10);

    m_shape.m_pos += m_vel;
  }
  void draw(SDL_Renderer* renderer) override
  {
    gk::Draw::setRendererColor(renderer, gk::Color::OLIVE);
    gk::Draw::filledCircle(renderer, m_shape.m_pos.GetX<int>(),
                           m_shape.m_pos.GetY<int>(),
                           m_shape.m_size.GetX<int>());
    m_textBox.draw(renderer);
  }

private:
  void paused(const gk::event_details&)
  {
    m_sharedContext->state_machine->switchTo(StateType::PAUSED);
    m_sharedContext->inputHandler->set_current_state(StateType::PAUSED);
  }
  void mouseMotion(const gk::event_details& details)
  {
    m_mousePos = details.mouse_pos;
  }

  gk::SharedContextPtr m_sharedContext{nullptr};
  Shape m_shape;
  gk::vector2d m_mousePos;
  gk::vector2d m_vel;
  gk::TextBox m_textBox;
  TTF_Font* m_font{nullptr};
};

class PauseState : public gk::base_state
{
  struct Shape
  {
    gk::vector2d m_pos{0, 0};
    gk::vector2d m_size{0, 0};
  };

public:
  PauseState(gk::SharedContextPtr sharedContext)
      : m_sharedContext{sharedContext}
  {
  }

  void on_create() override
  {
    set_transparent(true);

    m_shape.m_size = {640, 480};
    m_shape.m_pos = {0, 0};

    {
      m_sharedContext->inputHandler->add_callback(
          StateType::PAUSED, "Unpause",
          [this](const gk::event_details& details) { unpause(details); });

      auto binding = gk::event_binding{"Unpause"};
      binding.events.push_back({gk::EventType::KeyDown, SDL_SCANCODE_P});
      m_sharedContext->inputHandler->add_binding(StateType::PAUSED, binding);
    }
  }
  void on_destroy() override
  {
    m_sharedContext->inputHandler->remove_binding(StateType::PAUSED, "Unpause");
    m_sharedContext->inputHandler->remove_callback(StateType::PAUSED,
                                                   "Unpause");
  }

  void activate() override
  {
  }
  void deactivate() override
  {
  }

  void update() override
  {
  }
  void draw(SDL_Renderer* renderer) override
  {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 50);
    gk::Draw::filledRect(renderer, m_shape.m_pos, m_shape.m_size);
  }

private:
  void unpause(const gk::event_details&)
  {
    m_sharedContext->state_machine->switchTo(StateType::GAME);
    m_sharedContext->inputHandler->set_current_state(StateType::GAME);
  }

  gk::SharedContextPtr m_sharedContext{nullptr};
  Shape m_shape;
};

int main()
{
  const gk::vector2d windowSize{640, 480};

  auto app = std::make_shared<gk::App>(gk::AppConfiguration{
      "state_machine", static_cast<size_t>(windowSize.GetX<int>()),
      static_cast<size_t>(windowSize.GetY<int>())});

  auto inputHandler = std::make_shared<gk::state_input_handler>();
  auto state_machine = std::make_shared<gk::state_machine>();
  gk::SharedContextPtr sharedContext =
      std::make_shared<gk::SharedContext>(inputHandler, state_machine, app);

  state_machine->registerState(
      StateType::INTRO,
      [sharedContext, &windowSize]() -> gk::base_state_ptr
      { return std::make_unique<IntroState>(sharedContext, windowSize); });

  state_machine->registerState(
      StateType::MAIN,
      [sharedContext]() -> gk::base_state_ptr
      { return std::make_unique<MainState>(sharedContext); });

  state_machine->registerState(
      StateType::GAME,
      [sharedContext]() -> gk::base_state_ptr
      { return std::make_unique<GameState>(sharedContext); });

  state_machine->registerState(
      StateType::PAUSED,
      [sharedContext]() -> gk::base_state_ptr
      { return std::make_unique<PauseState>(sharedContext); });

  app->setInputHandler(inputHandler);
  app->setstate_machine(state_machine);
  state_machine->switchTo(StateType::INTRO);

  try
  {
    app->run();
  }
  catch (const std::exception& e)
  {
    spdlog::error(e.what());
  }
  return 0;
}