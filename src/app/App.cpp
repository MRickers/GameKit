#include "app/App.hpp"
#include "core/InputHandler.hpp"
#include "helpers/Draw.hpp"
#include "helpers/Timer.hpp"
#include <SDL2/SDL.h>

#include "App.hpp"
#include <chrono>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <thread>

namespace gk
{

  App::App(const AppConfiguration& config)
  {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                   "SDL could not initialize! SDL_Error: %s", SDL_GetError());
      throw std::runtime_error{"SDL could not initialize"};
    }
    const auto& [width, height] = config.getSize().Get();

    if (m_window = SDL_CreateWindow(
            config.getTitle().c_str(), SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
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

  App::App(InputHandlerPtr input_handler, const AppConfiguration& config)
      : App(config)
  {
    setInputHandler(std::move(input_handler));
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
    SDL_Event evnt;
    while (SDL_PollEvent(&evnt) != 0)
    {
      if (m_input_handler)
      {
        m_input_handler->HandleEvent(evnt);
      }
      if (evnt.type == SDL_QUIT)
      {
        stop();
      }
    }
  }

  void App::update()
  {
    if (m_input_handler)
    {
      m_input_handler->Update();
    }
  }

  void App::draw()
  {
    static auto x = 320;
    // clear renderer
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    // draw circle
    gk::Draw::setRendererColor(m_renderer, gk::Color::CRIMSON);
    gk::Draw::filledCircle(m_renderer, x++, 240, 150);
    if (x >= 640)
    {
      x = 0;
    }
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
    // const time_ms update_rate = 1000 / 60;
    const auto update_rate = std::chrono::milliseconds{1000 / 60};
    const auto max_updates = 5;

    auto lastUpdateTime = std::chrono::system_clock::now();
    auto lastRenderTime = std::chrono::system_clock::now();

    auto fps_timer = gk::Timer{};
    fps_timer.Start();
    auto framecount = 0;

    while (m_running)
    {
      auto updates = 0;
      auto now = std::chrono::system_clock::now();

      while (std::chrono::duration_cast<std::chrono::milliseconds>(
                 now - lastUpdateTime) > update_rate &&
             updates < max_updates)
      {
        handleEvents();
        update();
        lastUpdateTime += update_rate;
        updates++;
        framecount++;
      }

      // if maybe an update tooks forever we dont want to catch up
      if (now - lastUpdateTime > update_rate)
      {
        lastUpdateTime = now - update_rate;
      }

      draw();
      lastRenderTime = now;

      if (fps_timer.HasPassed(1000))
      {
        spdlog::info("{}", framecount);
        framecount = 0;
        fps_timer.Reset();
      }

      while ((now - lastRenderTime) < update_rate &&
             (now - lastUpdateTime) < update_rate)
      {
        std::this_thread::sleep_for(std::chrono::milliseconds{1});
        now = std::chrono::system_clock::now();
      }
    }
  }

  void gk::App::setInputHandler(InputHandlerPtr input_handler)
  {
    m_input_handler.reset(input_handler.release());
  }
} // namespace gk
