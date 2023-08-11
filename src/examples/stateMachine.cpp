#include "app/App.hpp"
#include "helpers/Draw.hpp"
#include "helpers/Timer.hpp"
#include <iostream>

enum class StateType
{
  INTRO,
  MAIN,
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

int main()
{
  const gk::Vector2D windowSize{640, 480};

  gk::App app{{"StateMachine", static_cast<size_t>(windowSize.GetX()),
               static_cast<size_t>(windowSize.GetY())}};

  auto inputHandler = std::make_shared<gk::StateInputHandler>();
  auto stateMachine = std::make_shared<gk::StateMachine>();
  gk::SharedContextPtr sharedContext =
      std::make_shared<gk::SharedContext>(inputHandler, stateMachine);

  stateMachine->registerState(
      StateType::INTRO,
      [sharedContext, &windowSize]() -> gk::BaseStatePtr
      { return std::make_unique<IntroState>(sharedContext, windowSize); });

  app.setInputHandler(inputHandler);
  app.setStateMachine(stateMachine);

  try
  {
    app.run();
  }
  catch (const std::exception& e)
  {
    std::cout << e.what();
  }
  return 0;
}