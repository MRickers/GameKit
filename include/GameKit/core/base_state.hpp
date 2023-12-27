#pragma once

class SDL_Renderer;

namespace gk
{
  class base_state
  {
  public:
    virtual ~base_state()
    {
    }
    virtual void onCreate() = 0;
    virtual void onDestroy() = 0;

    virtual void activate() = 0;
    virtual void deactivate() = 0;

    virtual void update() = 0;
    virtual void draw(SDL_Renderer*) = 0;

    void setTransparent(const bool transparent)
    {
      m_transparent = transparent;
    }

    void setTranscendent(const bool transcendent)
    {
      m_transcendent = transcendent;
    }

    bool isTransparent() const
    {
      return m_transparent;
    }

    bool isTranscendent() const
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
