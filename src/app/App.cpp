#include "app/App.hpp"
#include "core/InputHandler.hpp"
#include "core/ui/TextBox.hpp"
#include "helpers/Draw.hpp"
#include "helpers/Timer.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "App.hpp"
#include <chrono>
#include <stdexcept>
#include <thread>

namespace gk
{

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

  App::App(InputHandlerPtr input_handler, StateMachinePtr stateMachine,
           const AppConfiguration& config)
      : App(config)
  {
    setInputHandler(input_handler);
    setStateMachine(stateMachine);
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
      if (m_inputHandler)
      {
        m_inputHandler->HandleEvent(evnt);
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
      m_inputHandler->Update();
    }
    if (m_stateMachine)
    {
      m_stateMachine->update();
    }
  }

  void App::draw()
  {
    if (m_stateMachine)
    {
      m_stateMachine->draw(m_renderer);
    }

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
    gk::TextBox fpsText{1};

    fpsText.setPos(gk::Vector2D{2, m_size.GetY() - 2 - fontSize});
    if (auto* font = TTF_OpenFont("/home/mrickers/projects/c++/"
                                  "sdl2_snake/assets/Roboto-Regular.ttf",
                                  fontSize);
        font != nullptr)
    {

      fpsText.setFont(font, fontSize);
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
      fpsText.draw(m_renderer);
      draw();

      lastRenderTime = now;

      if (m_stateMachine)
      {
        m_stateMachine->processRequests();
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

  void gk::App::setInputHandler(InputHandlerPtr inputHandler)
  {
    if (inputHandler)
    {
      m_inputHandler = inputHandler;
    }
  }

  void App::setStateMachine(StateMachinePtr stateMachine)
  {
    if (stateMachine)
    {
      m_stateMachine = stateMachine;
    }
  }
} // namespace gk
