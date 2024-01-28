#include "GameKit/App.hpp"
#include <iostream>

int main()
{
  try
  {
    gk::App app{{"Snake", 640, 480}};
    auto& input = app.get_input_handler();

    input->add_global_keydown_binding("test", {SDL_SCANCODE_A});
    input->add_global_keydown_repeat_binding(
        "strg+c", {SDL_SCANCODE_LCTRL, SDL_SCANCODE_C});
    input->add_global_keydown_binding("esc", {SDL_SCANCODE_ESCAPE});
    input->add_global_keydown_binding("leftMouse", {}, {gk::MouseButton::Left});
    input->add_global_keydown_binding("rightMouse", {},
                                      {gk::MouseButton::Right});
    input->add_global_keydown_repeat_binding(
        "motion", {}, {gk::MouseButton::Left, gk::MouseButton::Motion});
    input->add_global_keydown_repeat_binding(
        "strg+motion", {SDL_SCANCODE_LCTRL}, {gk::MouseButton::Motion});

    input->add_global_callback("test", [](const gk::event_details&)
                               { std::cout << "test\n"; });

    input->add_global_callback("strg+c", [](const gk::event_details&)
                               { std::cout << "strg+c\n"; });
    input->add_global_callback("esc", [&app](const gk::event_details&)
                               { app.stop(); });

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
                                 std::cout << "STRG+motion: "
                                           << b.mouse_pos.GetX<int>() << ","
                                           << b.mouse_pos.GetY<int>() << "\n";
                               });

    app.run();
  }
  catch (...)
  {
    std::cout << "exception occured\n";
  }
  gk::App::shutdown();
  return 0;
}