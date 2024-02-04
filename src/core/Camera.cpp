//
// Created by max on 04.02.24.
//

#include "GameKit/core/Camera.hpp"
#include "GameKit/helpers/GameException.hpp"

namespace gk {
  Camera::Camera(Vector2d const& t_screen_size, Vector2d const& t_world_size)
      : m_screen_size{t_screen_size}
      , m_world_size{t_world_size}
      , m_position{0, 0}
      , m_target{nullptr} {
  }
  void Camera::set_target(Rect* const t_target) {
    if (t_target) {
      m_target = t_target;
    }
    throw GameException("could not net target", 2000);
  }
  void Camera::set_position(Vector2d const& new_pos) {
    m_position = new_pos;
  }
  Vector2d Camera::transform(Vector2d current_position) const {
    current_position -= m_position;
    return current_position;
  }
  void Camera::update() {
    if (m_target) {
      m_position = {
          (m_target->pos.GetX<int>() + m_target->size.GetX<int>() / 2) -
              m_screen_size.GetX<int>() / 2,
          (m_target->pos.GetX<int>() + m_target->size.GetX<int>() / 2) -
              m_screen_size.GetY<int>() / 2};
    }
    auto const [camera_x, camera_y] = m_position.Get();
    if (camera_x < 0) {
      set_position({0, m_position.GetY<int>()});
    } else if (camera_y < 0) {
      set_position({m_position.GetX<int>(), 0});
    } else if (static_cast<int>(camera_x) >
               m_world_size.GetX<int>() - m_position.GetX<int>()) {
      set_position({m_world_size.GetX<int>() - m_position.GetX<int>(),
                    m_position.GetY<int>()});
    } else if (static_cast<int>(camera_y) >
               m_world_size.GetY<int>() - m_position.GetY<int>()) {
      set_position({m_position.GetX<int>(),
                    m_world_size.GetY<int>() - m_position.GetY<int>()});
    }
  }
} // namespace gk