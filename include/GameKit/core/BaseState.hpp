#pragma once

class SDL_Renderer;

namespace gk
{
  class BaseState
  {
  public:
    virtual ~BaseState()
    {
    }
    virtual void on_create() = 0;
    virtual void on_destroy() = 0;

    virtual void activate() = 0;
    virtual void deactivate() = 0;

    virtual void update() = 0;
    virtual void draw(SDL_Renderer*) = 0;

    void set_transparent(const bool transparent)
    {
      m_transparent = transparent;
    }

    void set_transcendent(const bool transcendent)
    {
      m_transcendent = transcendent;
    }

    bool is_transparent() const
    {
      return m_transparent;
    }

    bool is_transcendent() const
    {
      return m_transcendent;
    }

  private:
    // does current state allow others to be drawn
    bool m_transparent{false};
    // does current state allow others to be updated
    bool m_transcendent{false};
  };

} // namespace gk
