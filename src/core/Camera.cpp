//
// Created by max on 04.02.24.
//

#include "GameKit/core/Camera.hpp"
#include "GameKit/helpers/GameException.hpp"

namespace gk {
  Camera::Camera(Vector2d const& t_screen_size)
      : m_screen_size{t_screen_size}
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
          m_target->pos.GetX<int>() +
              (m_target->size.GetX<int>() - m_screen_size.GetX<int>()) / 2,
          m_target->pos.GetY<int>() +
              (m_target->size.GetY<int>() - m_screen_size.GetY<int>()) / 2};
    }
  }
} // namespace gk