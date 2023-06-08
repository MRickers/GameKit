#include <SDL2/SDL.h>

#include "app/App.hpp"

int main()
{
  gk::App app{"Snake", 640, 480};
  app.run();
  return 0;
}