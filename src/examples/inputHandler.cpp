#include "GameKit/App.hpp"
#include <iostream>

int main()
{
  gk::App app{{"Snake", 640, 480}};
  gk::InputHandlerPtr input = std::make_shared<gk::StateInputHandler>();

  {
    auto binding = gk::EventBinding{"test"};
    binding.events.push_back({gk::EventType::KeyDown, SDL_SCANCODE_A});

    input->AddGlobalBinding(binding);
  }
  {
    auto binding = gk::EventBinding{"strg+c"};
    binding.events.push_back({gk::EventType::KeyDown, SDL_SCANCODE_LCTRL});
    binding.events.push_back({gk::EventType::KeyDown, SDL_SCANCODE_C});

    input->AddGlobalBinding(binding);
  }
  {
    auto binding = gk::EventBinding{"esc"};
    binding.events.push_back({gk::EventType::KeyDown, SDL_SCANCODE_ESCAPE});

    input->AddGlobalBinding(binding);
  }
  {
    auto binding = gk::EventBinding{"leftMouse"};
    binding.events.push_back({gk::EventType::MouseDown});

    input->AddGlobalBinding(binding);
  }
  {
    auto binding = gk::EventBinding{"rightMouse"};
    binding.events.push_back({gk::EventType::MouseDown, SDL_SCANCODE_UNKNOWN,
                              gk::MouseButton::Right});

    input->AddGlobalBinding(binding);
  }
  {
    auto binding = gk::EventBinding{"motion"};
    binding.events.push_back({gk::EventType::MouseDown, SDL_SCANCODE_UNKNOWN,
                              gk::MouseButton::Left});
    binding.events.push_back({gk::EventType::MouseMotion});

    input->AddGlobalBinding(binding);
  }
  {
    auto binding = gk::EventBinding{"strg+motion"};
    binding.events.push_back({gk::EventType::KeyDown, SDL_SCANCODE_LCTRL});
    binding.events.push_back({gk::EventType::MouseMotion});

    input->AddGlobalBinding(binding);
  }
  input->AddGlobalCallback("test", [](const gk::EventDetails&)
                           { std::cout << "test\n"; });

  input->AddGlobalCallback("strg+c", [](const gk::EventDetails&)
                           { std::cout << "strg+c\n"; });
  input->AddGlobalCallback("esc",
                           [&app](const gk::EventDetails&) { app.stop(); });

  input->AddGlobalCallback("leftMouse",
                           [&app](const gk::EventDetails& b)
                           {
                             std::cout
                                 << "left mouse pressed: " << b.mouse_pos.GetX()
                                 << "," << b.mouse_pos.GetY() << "\n";
                           });
  input->AddGlobalCallback("rightMouse",
                           [&app](const gk::EventDetails& b)
                           {
                             std::cout << "right mouse pressed: "
                                       << b.mouse_pos.GetX() << ","
                                       << b.mouse_pos.GetY() << "\n";
                           });
  input->AddGlobalCallback("motion",
                           [&app](const gk::EventDetails& b)
                           {
                             std::cout << "motion: " << b.mouse_pos.GetX()
                                       << "," << b.mouse_pos.GetY() << "\n";
                           });
  input->AddGlobalCallback("strg+motion",
                           [&app](const gk::EventDetails& b)
                           {
                             std::cout << "STRG+motion: " << b.mouse_pos.GetX()
                                       << "," << b.mouse_pos.GetY() << "\n";
                           });

  app.setInputHandler(input);
  app.run();
  return 0;
}