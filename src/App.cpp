#include "GameKit/App.hpp"
#include "GameKit/core/input_handler.hpp"
#include "GameKit/core/ui/text_box.hpp"
#include "GameKit/helpers/Draw.hpp"
#include "GameKit/helpers/Timer.hpp"
#include "GameKit/vector/vector2d.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <chrono>
#include <stdexcept>
#include <thread>

namespace gk
{
  static gk::TextBox fpsText{1};

  App::App(const AppConfiguration& config)
  {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                   "SDL could not initialize SDL! SDL_Error: %s",
                   SDL_GetError());
      throw std::runtime_error{"SDL could not initialize"};
    }
    if (TTF_Init() < 0)
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                   "SDL could not initialize TTF! SDL_Error: %s",
                   TTF_GetError());
      throw std::runtime_error{"SDL could not initialize ttf"};
    }
    const auto& [width, height] = config.getSize().Get();
    m_size = config.getSize();

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
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
  }

  App::App(input_handler_ptr input_handler, state_machinePtr state_machine,
           const AppConfiguration& config)
      : App(config)
  {
    setInputHandler(input_handler);
    setstate_machine(state_machine);
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
    TTF_Quit();
    SDL_Quit();
  }

  void App::handleEvents()
  {
    SDL_Event evnt;
    while (SDL_PollEvent(&evnt) != 0)
    {
      if (m_inputHandler)
      {
        m_inputHandler->handle_event(evnt);
      }
      if (evnt.type == SDL_QUIT)
      {
        stop();
      }
    }
  }

  void App::update()
  {
    if (m_inputHandler)
    {
      m_inputHandler->update();
    }
    if (m_state_machine)
    {
      m_state_machine->update();
    }
  }

  void App::draw()
  {
    if (m_state_machine)
    {
      m_state_machine->draw(m_renderer);
    }

    fpsText.draw(m_renderer);

    SDL_RenderPresent(m_renderer);
  }

  void App::clearRenderer()
  {
    // clear renderer
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);
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
    constexpr auto fontSize = 20;
#ifndef NDEBUG
    constexpr auto fontPath = "Roboto-Regular.ttf";
#else
    constexpr auto fontPath = "Roboto-Regular.ttf";
#endif

    fpsText.setPos(gk::vector2d{2.f, m_size.GetY<float>() - 2 - fontSize});
    if (auto* font = TTF_OpenFont(fontPath, fontSize); font != nullptr)
    {
      fpsText.setFont(font, fontSize);
    }
    else
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                   "could not load fps font from: %s", fontPath);
    }
    m_running = true;
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
      if (std::chrono::duration_cast<std::chrono::milliseconds>(
              now - lastUpdateTime) > update_rate)
      {
        lastUpdateTime = now - update_rate;
      }

      clearRenderer();

      if (fps_timer.HasPassed(1000))
      {
        fpsText.add("fps: " + std::to_string(framecount));
        framecount = 0;
        fps_timer.Reset();
      }
      draw();

      lastRenderTime = now;

      if (m_state_machine)
      {
        m_state_machine->processRequests();
      }

      while (std::chrono::duration_cast<std::chrono::milliseconds>(
                 now - lastRenderTime) < update_rate &&
             std::chrono::duration_cast<std::chrono::milliseconds>(
                 now - lastUpdateTime) < update_rate)
      {
        std::this_thread::sleep_for(std::chrono::milliseconds{1});
        now = std::chrono::system_clock::now();
      }
    }
  }

  void gk::App::setInputHandler(input_handler_ptr inputHandler)
  {
    if (inputHandler)
    {
      m_inputHandler = inputHandler;
    }
  }

  void App::setstate_machine(state_machinePtr state_machine)
  {
    if (state_machine)
    {
      m_state_machine = state_machine;
    }
  }
} // namespace gk

gk::vector2d gk::App::getWindowSize() const
{
  int width, height;
  SDL_GetWindowSize(m_window, &width, &height);

  return {width, height};
}
