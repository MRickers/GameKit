//
// Created by max on 04.02.24.
//

#ifndef GAMEKIT_CAMERA_HPP
#define GAMEKIT_CAMERA_HPP
#include "GameKit/helpers/Types.hpp"
#include "GameKit/vector/Vector2d.hpp"

namespace gk {

  class Camera {
  public:
    explicit Camera(Vector2d const & t_screen_size);
    void set_target(Rect* const t_target);
    void set_position(Vector2d const & new_pos);
    [[nodiscard]] Vector2d transform(Vector2d current_position) const;
    void update();

  private:
    Vector2d m_screen_size;
    Vector2d m_position;
    Rect* m_target{nullptr};
  };


} // namespace gk

#endif // GAMEKIT_CAMERA_HPP
