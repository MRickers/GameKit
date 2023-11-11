#pragma once
#include "vector/Vector2d.hpp"

class SDL_Renderer;
namespace gk
{
  class LoadParams
  {
  public:
    LoadParams(const Vector2D& pos, const Vector2D& size)
        : m_pos{pos}
        , m_size{size}
    {
    }

  private:
    Vector2D m_pos{0, 0};
    Vector2D m_size{0, 0};
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
