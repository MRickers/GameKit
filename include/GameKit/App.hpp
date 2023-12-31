#pragma once
#include "GameKit/core/state_input_handler.hpp"
#include "GameKit/core/state_machine.hpp"
#include "GameKit/core/texture_manager.hpp"
#include "vector/vector2d.hpp"
#include <memory>
#include <string>

class SDL_Renderer;
class SDL_Window;
namespace gk
{
  using input_handler_ptr = std::shared_ptr<state_input_handler>;
  using state_machinePtr = std::shared_ptr<state_machine>;
  using texture_manager_ptr = std::shared_ptr<texture_manager>;

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

  class App;
  struct SharedContext
  {
    state_input_handler* inputHandler_ptr{nullptr};
    state_machine* state_machine_ptr{nullptr};
    texture_manager* texture_manager_ptr{nullptr};
    App* app{nullptr};
  };

  class App
  {
  public:
    explicit App(const AppConfiguration& config);
    ~App();

    void stop();
    void run();
    SDL_Renderer* get_renderer() const;

    gk::vector2d getWindowSize() const;
    gk::SharedContext get_shared_context() const;
    std::unique_ptr<state_input_handler> const& get_input_handler() const;
    std::unique_ptr<state_machine> const& get_state_machine() const;
    static void shutdown();

  private:
    void handleEvents();
    void update();
    void draw();
    void clearRenderer();
    bool isRunning() const;

    bool m_running{false};
    SDL_Window* m_window{nullptr};
    SDL_Renderer* m_renderer{nullptr};
    std::unique_ptr<state_input_handler> m_inputHandler{nullptr};
    std::unique_ptr<state_machine> m_state_machine{nullptr};
    std::unique_ptr<texture_manager> m_texture_manager{nullptr};

    gk::vector2d m_size;
    SharedContext m_shared_context;
  };

  using SharedContextPtr = std::shared_ptr<SharedContext>;

} // namespace gk
