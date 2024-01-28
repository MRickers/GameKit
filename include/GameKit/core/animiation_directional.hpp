#pragma once
#include "GameKit/core/base_animation.hpp"

namespace gk
{
  class animation_directional : public base_animation
  {
  public:
    explicit animation_directional(sprite_sheet*);
    void frame_step() override;
    void crop_sprite() override;
    bool parse_file_input(std::stringstream& t_str);
  };

} // namespace gk
