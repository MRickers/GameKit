#pragma once

class SDL_Renderer;

namespace gk
{
  class IBaseState
  {
  public:
    virtual ~IBaseState()
    {
    }
    virtual void onCreate() = 0;
    virtual void onDestroy() = 0;

    virtual void activate() = 0;
    virtual void deactivate() = 0;

    virtual void update() = 0;
    virtual void draw(const SDL_Renderer*) = 0;
  };

} // namespace gk
