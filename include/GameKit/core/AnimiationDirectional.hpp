#pragma once
#include "GameKit/core/BaseAnimation.hpp"

namespace gk
{
  class AnimationDirectional : public BaseAnimation
  {
  public:
    explicit AnimationDirectional(SpriteSheet*);
    void frame_step() override;
    void crop_sprite() override;
    bool parse_file_input(std::stringstream& t_str);
  };

} // namespace gk
