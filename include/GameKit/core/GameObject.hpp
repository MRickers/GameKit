#pragma once
#include "vector/vector2d.hpp"

class SDL_Renderer;
namespace gk
{
  class LoadParams
  {
  public:
    LoadParams(const vector2d& pos, const vector2d& size)
        : m_pos{pos}
        , m_size{size}
    {
    }

  private:
    vector2d m_pos{0, 0};
    vector2d m_size{0, 0};
  };

  class IGameObject
  {
  public:
    virtual ~IGameObject()
    {
    }
    virtual void Draw(const SDL_Renderer*) = 0;
    virtual void Update() = 0;
    virtual void Clean() = 0;
  };

} // namespace gk
