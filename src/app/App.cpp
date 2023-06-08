#include "app/App.hpp"
#include "helpers/Draw.hpp"
#include <SDL2/SDL.h>

#include <stdexcept>
namespace gk
{

  App::App(const std::string& title, size_t width, size_t height)
  {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                   "SDL could not initialize! SDL_Error: %s", SDL_GetError());
      throw std::runtime_error{"SDL could not initialize"};
    }
    if (m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED, width, height,
                                    SDL_WINDOW_SHOWN);
        m_window == nullptr)
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                   "SDL could not initialize window! SDL_Error: %s",
                   SDL_GetError());
      throw std::runtime_error{"SDL initialize window failed"};
    }
    if (m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
        m_renderer == nullptr)
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                   "SDL could not initialize renderer! SDL_Error: %s",
                   SDL_GetError());
      throw std::runtime_error{"SDL initialize renderer failed"};
    }
  }

  App::~App()
  {
    if (m_renderer != nullptr)
    {
      SDL_DestroyRenderer(m_renderer);
    }
    if (m_window != nullptr)
    {
      SDL_DestroyWindow(m_window);
    }
    SDL_Quit();
  }

  void App::handleEvents()
  {
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0)
    {
      if (e.type == SDL_QUIT)
      {
        m_running = false;
      }
    }
  }

  void App::update()
  {
  }

  void App::draw()
  {
    // clear renderer
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    // draw circle
    gk::Draw::setRendererColor(m_renderer, gk::Color::CRIMSON);
    gk::Draw::filledCircle(m_renderer, 320, 240, 150);
    // draw
    SDL_RenderPresent(m_renderer);
  }

  bool App::isRunning() const
  {
    return m_running;
  }

  void App::stop()
  {
    m_running = false;
  }
  void App::run()
  {
    m_running = true;

    while (m_running)
    {
      handleEvents();
      update();
      draw();
    }
  }
} // namespace gk
