#include "app/App.hpp"
#include "helpers/Draw.hpp"
#include "helpers/Timer.hpp"
#include <SDL2/SDL.h>

#include <spdlog/spdlog.h>
#include <stdexcept>

#include <algorithm>
#include <chrono>
#include <thread>

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
  // void App::run()
  // {
  //   m_running = true;
  //   const time_ms update_rate = 1000 / 60;
  //   const auto max_updates = 5;
  //   auto update_clock = gk::Timer{};
  //   update_clock.Start();

  //   auto counted_frames = 0;
  //   auto fps_timer = gk::Timer{};
  //   fps_timer.Start();

  //   // fixed timestep ohne Timer Klasse bauen und schauen, ob fps erreich
  //   wird while (m_running)
  //   {
  //     auto updates = 0;

  //     if (auto elapsed_time = update_clock.Round(); elapsed_time >=
  //     update_rate)
  //     {
  //       while (elapsed_time >= update_rate && updates++ < max_updates)
  //       {
  //         handleEvents();
  //         update();
  //         elapsed_time -= update_rate;
  //         ++counted_frames;
  //       }
  //       update_clock.Reset();
  //     }
  //     draw();

  //     if (fps_timer.HasPassed(1000))
  //     {
  //       spdlog::info("Fps: {}", counted_frames);
  //       counted_frames = 0;
  //       fps_timer.Reset();
  //     }
  //     // Sleep einbauen wegen cpu
  //   }
  // }
} // namespace gk
