#pragma once
#include "core/StateInputHandler.hpp"
#include "core/StateMachine.hpp"
#include "vector/vector2d.hpp"
#include <memory>
#include <string>

class SDL_Renderer;
class SDL_Window;
namespace gk
{
  using InputHandlerPtr = std::shared_ptr<StateInputHandler>;
  using StateMachinePtr = std::shared_ptr<StateMachine>;

  class AppConfiguration
  {
  public:
    AppConfiguration(const std::string& title, size_t width, size_t height)
        : m_title{title}
        , m_size{static_cast<float>(width), static_cast<float>(height)}
    {
    }
    std::string getTitle() const
    {
      return m_title;
    }
    vector2d getSize() const
    {
      return m_size;
    }

  private:
    std::string m_title{"Title"};
    gk::vector2d m_size{640, 480};
  };
  class App
  {
  public:
    App(InputHandlerPtr input_handler, StateMachinePtr stateMachine,
        const AppConfiguration& config);
    App(const AppConfiguration& config);
    ~App();

    void stop();
    void run();

    void setInputHandler(InputHandlerPtr input_handler);
    void setStateMachine(StateMachinePtr stateMachine);

    gk::vector2d getWindowSize() const;

  private:
    void handleEvents();
    void update();
    void draw();
    void clearRenderer();
    bool isRunning() const;

    bool m_running{false};
    SDL_Window* m_window{nullptr};
    SDL_Renderer* m_renderer{nullptr};
    InputHandlerPtr m_inputHandler{nullptr};
    StateMachinePtr m_stateMachine{nullptr};
    gk::vector2d m_size;
  };

  using AppPtr = std::shared_ptr<App>;

  struct SharedContext
  {
    SharedContext(InputHandlerPtr ih, StateMachinePtr sm, AppPtr a)
        : inputHandler{ih}
        , stateMachine{sm}
        , app{a}
    {
    }

    InputHandlerPtr inputHandler{nullptr};
    StateMachinePtr stateMachine{nullptr};
    AppPtr app{nullptr};
  };

  using SharedContextPtr = std::shared_ptr<SharedContext>;

} // namespace gk
