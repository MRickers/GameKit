#include "GameKit/App.hpp"
#include <iostream>

int main()
{
  gk::App app{{"Snake", 640, 480}};
  gk::InputHandlerPtr input = std::make_shared<gk::StateInputHandler>();

  {
    auto binding = gk::event_binding{"test"};
    binding.events.push_back({gk::EventType::KeyDown, SDL_SCANCODE_A});

    input->AddGlobalBinding(binding);
  }
  {
    auto binding = gk::event_binding{"strg+c"};
    binding.events.push_back({gk::EventType::KeyDown, SDL_SCANCODE_LCTRL});
    binding.events.push_back({gk::EventType::KeyDown, SDL_SCANCODE_C});

    input->AddGlobalBinding(binding);
  }
  {
    auto binding = gk::event_binding{"esc"};
    binding.events.push_back({gk::EventType::KeyDown, SDL_SCANCODE_ESCAPE});

    input->AddGlobalBinding(binding);
  }
  {
    auto binding = gk::event_binding{"leftMouse"};
    binding.events.push_back({gk::EventType::MouseDown});

    input->AddGlobalBinding(binding);
  }
  {
    auto binding = gk::event_binding{"rightMouse"};
    binding.events.push_back({gk::EventType::MouseDown, SDL_SCANCODE_UNKNOWN,
                              gk::MouseButton::Right});

    input->AddGlobalBinding(binding);
  }
  {
    auto binding = gk::event_binding{"motion"};
    binding.events.push_back({gk::EventType::MouseDown, SDL_SCANCODE_UNKNOWN,
                              gk::MouseButton::Left});
    binding.events.push_back({gk::EventType::MouseMotion});

    input->AddGlobalBinding(binding);
  }
  {
    auto binding = gk::event_binding{"strg+motion"};
    binding.events.push_back({gk::EventType::KeyDown, SDL_SCANCODE_LCTRL});
    binding.events.push_back({gk::EventType::MouseMotion});

    input->AddGlobalBinding(binding);
  }
  input->AddGlobalCallback("test", [](const gk::event_details&)
                           { std::cout << "test\n"; });

  input->AddGlobalCallback("strg+c", [](const gk::event_details&)
                           { std::cout << "strg+c\n"; });
  input->AddGlobalCallback("esc",
                           [&app](const gk::event_details&) { app.stop(); });

  input->AddGlobalCallback("leftMouse",
                           [&app](const gk::event_details& b)
                           {
                             std::cout << "left mouse pressed: "
                                       << b.mouse_pos.GetX<int>() << ","
                                       << b.mouse_pos.GetY<int>() << "\n";
                           });
  input->AddGlobalCallback("rightMouse",
                           [&app](const gk::event_details& b)
                           {
                             std::cout << "right mouse pressed: "
                                       << b.mouse_pos.GetX<int>() << ","
                                       << b.mouse_pos.GetY<int>() << "\n";
                           });
  input->AddGlobalCallback("motion",
                           [&app](const gk::event_details& b)
                           {
                             std::cout << "motion: " << b.mouse_pos.GetX<int>()
                                       << "," << b.mouse_pos.GetY<int>() << "\n";
                           });
  input->AddGlobalCallback("strg+motion",
                           [&app](const gk::event_details& b)
                           {
                             std::cout
                                 << "STRG+motion: " << b.mouse_pos.GetX<int>()
                                 << "," << b.mouse_pos.GetY<int>() << "\n";
                           });

  app.setInputHandler(input);
  app.run();
  return 0;
}