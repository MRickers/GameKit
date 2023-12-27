#pragma once
#include "GameKit/core/state_input_handler.hpp"
#include "GameKit/core/state_machine.hpp"
#include "vector/vector2d.hpp"
#include <memory>
#include <string>

class SDL_Renderer;
class SDL_Window;
namespace gk
{
  using input_handler_ptr = std::shared_ptr<state_input_handler>;
  using state_machinePtr = std::shared_ptr<state_machine>;

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
    App(input_handler_ptr input_handler, state_machinePtr state_machine,
        const AppConfiguration& config);
    App(const AppConfiguration& config);
    ~App();

    void stop();
    void run();

    void setInputHandler(input_handler_ptr input_handler);
    void setstate_machine(state_machinePtr state_machine);

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
    input_handler_ptr m_inputHandler{nullptr};
    state_machinePtr m_state_machine{nullptr};
    gk::vector2d m_size;
  };

  using AppPtr = std::shared_ptr<App>;

  struct SharedContext
  {
    SharedContext(input_handler_ptr ih, state_machinePtr sm, AppPtr a)
        : inputHandler{ih}
        , state_machine{sm}
        , app{a}
    {
    }

    input_handler_ptr inputHandler{nullptr};
    state_machinePtr state_machine{nullptr};
    AppPtr app{nullptr};
  };

  using SharedContextPtr = std::shared_ptr<SharedContext>;

} // namespace gk
