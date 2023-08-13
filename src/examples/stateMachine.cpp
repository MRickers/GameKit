#include "app/App.hpp"
#include "helpers/Draw.hpp"
#include "helpers/Timer.hpp"
#include <iostream>

enum class StateType
{
  INTRO,
  MAIN,
  GAME,
};

class IntroState : public gk::IBaseState
{
public:
  IntroState(gk::SharedContextPtr sharedContext, const gk::Vector2D& windowSize)
      : m_sharedContext{sharedContext}
      , m_windowSize{windowSize}
  {
  }

  void onCreate() override
  {
    m_pos = m_windowSize / 2 - m_size / 2;

    m_sharedContext->inputHandler->AddCallback(
        StateType::INTRO, "IntroContinue",
        [this](const gk::EventDetails& details) { changeState(details); });

    auto binding = gk::EventBinding{"IntroContinue"};
    binding.events.push_back({gk::EventType::KeyDown, SDL_SCANCODE_SPACE});
    m_sharedContext->inputHandler->AddBinding(StateType::INTRO, binding);
  }
  void onDestroy() override
  {
    m_sharedContext->inputHandler->RemoveBinding(StateType::INTRO,
                                                 "IntroContinue");
    m_sharedContext->inputHandler->RemoveCallback(StateType::INTRO,
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
      m_pos += gk::Vector2D{0, 1};
    }
  }
  void draw(SDL_Renderer* renderer) override
  {
    gk::Draw::setRendererColor(renderer, gk::Color::MAROON);
    gk::Draw::filledRect(renderer, m_pos, m_size);
  }

private:
  void changeState(const gk::EventDetails&)
  {
    if (m_timer.HasPassed(2000))
    {
      if (m_sharedContext)
      {
        m_sharedContext->stateMachine->switchTo(StateType::MAIN);
        m_sharedContext->stateMachine->remove(StateType::INTRO);
        m_sharedContext->inputHandler->setCurrentState(StateType::MAIN);
      }
    }
  }

  gk::SharedContextPtr m_sharedContext{nullptr};
  gk::Timer m_timer;
  gk::Vector2D m_windowSize{0, 0};
  gk::Vector2D m_pos{0, 0};
  gk::Vector2D m_size{200, 100};
};

class MainState : public gk::IBaseState
{
  struct Shape
  {
    gk::Vector2D m_pos{0, 0};
    gk::Vector2D m_size{0, 0};
  };

public:
  MainState(gk::SharedContextPtr sharedContext)
      : m_sharedContext{sharedContext}
  {
  }

  void onCreate() override
  {
    const auto [x, y] = m_buttonPos.Get();
    for (int i = 0; i < 3; ++i)
    {
      const auto buttonPos =
          gk::Vector2D{x, y + (i * (m_buttonSize.GetY() + m_buttonPadding))};

      m_shapes[i].m_pos = buttonPos + m_buttonSize / 2;
      m_shapes[i].m_size = m_buttonSize;
    }
    m_sharedContext->inputHandler->AddCallback(
        StateType::MAIN, "MouseLeft",
        [this](const gk::EventDetails& details) { mouseClick(details); });

    auto binding = gk::EventBinding{"MouseLeft"};
    binding.events.push_back({gk::EventType::MouseDown, SDL_SCANCODE_UNKNOWN,
                              gk::MouseButton::Left});
    m_sharedContext->inputHandler->AddBinding(StateType::MAIN, binding);
  }
  void onDestroy() override
  {
    m_sharedContext->inputHandler->RemoveBinding(StateType::INTRO,
                                                 "IntroContinue");
    m_sharedContext->inputHandler->RemoveCallback(StateType::INTRO,
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
  void mouseClick(const gk::EventDetails& details)
  {
    const auto [mouseX, mouseY] = details.mouse_pos.Get();
    const auto [btnSizeX, btnSizeY] = m_buttonSize.Get();

    for (int i = 0; i < 3; ++i)
    {
      if (mouseX >= m_shapes[i].m_pos.GetX() &&
          mouseX <= m_shapes[i].m_pos.GetX() + btnSizeX &&
          mouseY >= m_shapes[i].m_pos.GetY() &&
          mouseY <= m_shapes[i].m_pos.GetY() + btnSizeY)
      {
        if (i == 0)
        {
          m_sharedContext->stateMachine->switchTo(StateType::GAME);
          m_sharedContext->inputHandler->setCurrentState(StateType::GAME);
        }
        else if (i == 1)
        {
          std::cout << "Options\n";
        }
        else
        {
          m_sharedContext->app->stop();
        }
      }
    }
  }

  gk::SharedContextPtr m_sharedContext{nullptr};

  gk::Vector2D m_buttonSize{100, 50};
  gk::Vector2D m_buttonPos{220, 90};
  uint32_t m_buttonPadding{10};
  Shape m_shapes[3];
};

int main()
{
  const gk::Vector2D windowSize{640, 480};

  auto app = std::make_shared<gk::App>(gk::AppConfiguration{
      "StateMachine", static_cast<size_t>(windowSize.GetX()),
      static_cast<size_t>(windowSize.GetY())});

  auto inputHandler = std::make_shared<gk::StateInputHandler>();
  auto stateMachine = std::make_shared<gk::StateMachine>();
  gk::SharedContextPtr sharedContext =
      std::make_shared<gk::SharedContext>(inputHandler, stateMachine, app);

  stateMachine->registerState(
      StateType::INTRO,
      [sharedContext, &windowSize]() -> gk::BaseStatePtr
      { return std::make_unique<IntroState>(sharedContext, windowSize); });

  stateMachine->registerState(
      StateType::MAIN,
      [sharedContext]() -> gk::BaseStatePtr
      { return std::make_unique<MainState>(sharedContext); }

  );

  app->setInputHandler(inputHandler);
  app->setStateMachine(stateMachine);
  stateMachine->switchTo(StateType::INTRO);

  try
  {
    app->run();
  }
  catch (const std::exception& e)
  {
    std::cout << e.what();
  }
  return 0;
}