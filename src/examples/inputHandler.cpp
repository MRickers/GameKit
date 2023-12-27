#include "GameKit/App.hpp"
#include <iostream>

int main()
{
  gk::App app{{"Snake", 640, 480}};
  gk::input_handler_ptr input = std::make_shared<gk::state_input_handler>();

  {
    auto binding = gk::event_binding{"test"};
    binding.events.push_back({gk::EventType::KeyDown, SDL_SCANCODE_A});

    input->add_global_binding(binding);
  }
  {
    auto binding = gk::event_binding{"strg+c"};
    binding.events.push_back({gk::EventType::KeyDown, SDL_SCANCODE_LCTRL});
    binding.events.push_back({gk::EventType::KeyDown, SDL_SCANCODE_C});

    input->add_global_binding(binding);
  }
  {
    auto binding = gk::event_binding{"esc"};
    binding.events.push_back({gk::EventType::KeyDown, SDL_SCANCODE_ESCAPE});

    input->add_global_binding(binding);
  }
  {
    auto binding = gk::event_binding{"leftMouse"};
    binding.events.push_back({gk::EventType::MouseDown});

    input->add_global_binding(binding);
  }
  {
    auto binding = gk::event_binding{"rightMouse"};
    binding.events.push_back({gk::EventType::MouseDown, SDL_SCANCODE_UNKNOWN,
                              gk::MouseButton::Right});

    input->add_global_binding(binding);
  }
  {
    auto binding = gk::event_binding{"motion"};
    binding.events.push_back({gk::EventType::MouseDown, SDL_SCANCODE_UNKNOWN,
                              gk::MouseButton::Left});
    binding.events.push_back({gk::EventType::MouseMotion});

    input->add_global_binding(binding);
  }
  {
    auto binding = gk::event_binding{"strg+motion"};
    binding.events.push_back({gk::EventType::KeyDown, SDL_SCANCODE_LCTRL});
    binding.events.push_back({gk::EventType::MouseMotion});

    input->add_global_binding(binding);
  }
  input->add_global_callback("test", [](const gk::event_details&)
                             { std::cout << "test\n"; });

  input->add_global_callback("strg+c", [](const gk::event_details&)
                             { std::cout << "strg+c\n"; });
  input->add_global_callback("esc",
                             [&app](const gk::event_details&) { app.stop(); });

  input->add_global_callback("leftMouse",
                             [&app](const gk::event_details& b)
                             {
                               std::cout << "left mouse pressed: "
                                         << b.mouse_pos.GetX<int>() << ","
                                         << b.mouse_pos.GetY<int>() << "\n";
                             });
  input->add_global_callback("rightMouse",
                             [&app](const gk::event_details& b)
                             {
                               std::cout << "right mouse pressed: "
                                         << b.mouse_pos.GetX<int>() << ","
                                         << b.mouse_pos.GetY<int>() << "\n";
                             });
  input->add_global_callback("motion",
                             [&app](const gk::event_details& b)
                             {
                               std::cout
                                   << "motion: " << b.mouse_pos.GetX<int>()
                                   << "," << b.mouse_pos.GetY<int>() << "\n";
                             });
  input->add_global_callback("strg+motion",
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