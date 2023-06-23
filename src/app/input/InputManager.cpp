#include "InputManager.hpp"

void gk::InputManager::addCallback(const InputDetails& details,
                                   InputCallback callback)
{
}

void gk::InputManager::removeCallback(const std::string& name)
{
}

void gk::InputManager::handleEvents()
{
  SDL_Event e;

  while (SDL_PollEvent(&e) != 0)
  {
    if (e.type == SDL_QUIT)
    {
    }
  }
}
