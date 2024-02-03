#pragma once
#include "GameKit/core/StateInputHandler.hpp"
#include "GameKit/core/StateMachine.hpp"
#include "GameKit/core/TextureManager.hpp"
#include "vector/Vector2d.hpp"
#include <memory>
#include <string>

class SDL_Renderer;
class SDL_Window;
namespace gk
{
  using InputHandlerPtr = std::shared_ptr<StateInputHandler>;
  using StateMachinePtr = std::shared_ptr<StateMachine>;
  using TextureManagerPtr = std::shared_ptr<TextureManager>;

  class AppConfiguration
  {
  public:
    AppConfiguration(const std::string& title, size_t width, size_t height)
        : m_title{title}
        , m_size{static_cast<float>(width), static_cast<float>(height)}
    {
    }
    [[nodiscard]] std::string get_title() const
    {
      return m_title;
    }
    [[nodiscard]] Vector2d get_size() const
    {
      return m_size;
    }
    [[nodiscard]] std::chrono::milliseconds const& get_update_rate() const
    {
      return m_update_rate;
    }

  private:
    std::string m_title{"Title"};
    gk::Vector2d m_size{640, 480};
    std::chrono::milliseconds m_update_rate{1000 / 60};
  };

  class App;
  struct SharedContext
  {
    StateInputHandler* inputHandler_ptr{nullptr};
    StateMachine* state_machine_ptr{nullptr};
    TextureManager* texture_manager_ptr{nullptr};
    App* app{nullptr};
  };

  class App
  {
  public:
    explicit App(const AppConfiguration& config);
    ~App();

    void stop();
    void run();
    [[nodiscard]] SDL_Renderer* get_renderer() const;

    [[nodiscard]] gk::Vector2d get_window_size() const;
    [[nodiscard]] gk::SharedContext get_shared_context() const;
    std::unique_ptr<StateInputHandler>& get_input_handler();
    std::unique_ptr<StateMachine>& get_state_machine();
    std::unique_ptr<TextureManager>& get_texture_manager();
    [[nodiscard]] std::chrono::milliseconds const& get_update_rate() const;
    static void shutdown();

  private:
    void handle_events();
    void update();
    void draw();
    void clear_renderer();
    [[nodiscard]] bool is_running() const;
    std::chrono::milliseconds m_update_rate;
    bool m_running{false};
    SDL_Window* m_window{nullptr};
    SDL_Renderer* m_renderer{nullptr};
    std::unique_ptr<TextureManager> m_texture_manager{nullptr};
    std::unique_ptr<StateInputHandler> m_inputHandler{nullptr};
    std::unique_ptr<StateMachine> m_state_machine{nullptr};

    gk::Vector2d m_size;
    SharedContext m_shared_context;
  };

  using SharedContextPtr = std::shared_ptr<SharedContext>;

} // namespace gk
