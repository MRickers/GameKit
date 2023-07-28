#pragma once
#include "core/InputHandler.hpp"
#include "vector/Vector2d.hpp"
#include <memory>
#include <string>

class SDL_Renderer;
class SDL_Window;

namespace gk
{
  using InputHandlerPtr = std::unique_ptr<InputHandler>;
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
    Vector2D getSize() const
    {
      return m_size;
    }

  private:
    std::string m_title{"Title"};
    gk::Vector2D m_size{640, 480};
  };
  class App
  {
  public:
    App(InputHandlerPtr input_handler, const AppConfiguration& config);
    App(const AppConfiguration& config);
    ~App();

    void stop();
    void run();

    void setInputHandler(InputHandlerPtr input_handler);

  private:
    void handleEvents();
    void update();
    void draw();
    bool isRunning() const;

    bool m_running{false};
    SDL_Window* m_window{nullptr};
    SDL_Renderer* m_renderer{nullptr};
    InputHandlerPtr m_input_handler{nullptr};
  };

} // namespace gk
