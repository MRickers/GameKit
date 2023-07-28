#include "app/App.hpp"
#include <iostream>

int main()
{
  gk::App app{{"Snake", 640, 480}};
  gk::InputHandlerPtr input = std::make_unique<gk::InputHandler>();

  {
    auto binding = gk::EventBinding{"test"};
    binding.events.push_back({gk::EventType::KeyDown, SDL_SCANCODE_A});

    input->AddBinding(binding);
  }
  {
    auto binding = gk::EventBinding{"strg+c"};
    binding.events.push_back({gk::EventType::KeyDown, SDL_SCANCODE_LCTRL});
    binding.events.push_back({gk::EventType::KeyDown, SDL_SCANCODE_C});

    input->AddBinding(binding);
  }
  {
    auto binding = gk::EventBinding{"esc"};
    binding.events.push_back({gk::EventType::KeyDown, SDL_SCANCODE_ESCAPE});

    input->AddBinding(binding);
  }
  input->AddCallback("test",
                     [](const gk::EventDetails&) { std::cout << "test\n"; });

  input->AddCallback("strg+c",
                     [](const gk::EventDetails&) { std::cout << "strg+c\n"; });
  input->AddCallback("esc", [&app](const gk::EventDetails&) { app.stop(); });

  app.setInputHandler(std::move(input));
  app.run();
  return 0;
}