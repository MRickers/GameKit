#pragma once
#include <string>

class SDL_Renderer;
class SDL_Window;
namespace gk
{
  class App
  {
  public:
    App(const std::string& title, size_t width, size_t height);
    ~App();

    void stop();
    void run();

  private:
    void handleEvents();
    void update();
    void draw();

    bool m_running{false};
    SDL_Window* m_window{nullptr};
    SDL_Renderer* m_renderer{nullptr};
    bool isRunning() const;
  };

} // namespace gk
