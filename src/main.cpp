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
  {
    auto binding = gk::EventBinding{"leftMouse"};
    binding.events.push_back({gk::EventType::MouseDown});

    input->AddBinding(binding);
  }
  {
    auto binding = gk::EventBinding{"rightMouse"};
    binding.events.push_back({gk::EventType::MouseDown, SDL_SCANCODE_UNKNOWN,
                              gk::MouseButton::Right});

    input->AddBinding(binding);
  }
  {
    auto binding = gk::EventBinding{"motion"};
    binding.events.push_back({gk::EventType::MouseMotion});

    input->AddBinding(binding);
  }
  input->AddCallback("test",
                     [](const gk::EventDetails&) { std::cout << "test\n"; });

  input->AddCallback("strg+c",
                     [](const gk::EventDetails&) { std::cout << "strg+c\n"; });
  input->AddCallback("esc", [&app](const gk::EventDetails&) { app.stop(); });

  input->AddCallback("leftMouse",
                     [&app](const gk::EventDetails& b)
                     {
                       std::cout << "left mouse pressed: " << b.mouse_pos.GetX()
                                 << "," << b.mouse_pos.GetY() << "\n";
                     });
  input->AddCallback("rightMouse",
                     [&app](const gk::EventDetails& b)
                     {
                       std::cout
                           << "right mouse pressed: " << b.mouse_pos.GetX()
                           << "," << b.mouse_pos.GetY() << "\n";
                     });
  input->AddCallback("motion",
                     [&app](const gk::EventDetails& b)
                     {
                       std::cout << "motion: " << b.mouse_pos.GetX() << ","
                                 << b.mouse_pos.GetY() << "\n";
                     });

  app.setInputHandler(std::move(input));
  app.run();
  return 0;
}