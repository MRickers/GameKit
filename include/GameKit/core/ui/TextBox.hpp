#pragma once
#include <deque>
#include <string>
#include <vector>

#include "GameKit/helpers/Drawer.hpp"
#include "GameKit/vector/Vector2d.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace gk
{
  using text_container = std::vector<std::string>;
  using text_buffer = std::deque<std::string>;

  class TextCurcularBuffer
  {
  public:
    explicit TextCurcularBuffer(size_t maxRows = 6);
    void push(const std::string& content);
    void pop();
    void clear();
    [[nodiscard]] size_t size() const;
    [[nodiscard]] size_t rows() const;
    [[nodiscard]] text_container get() const;

  private:
    text_buffer m_textBuffer{};
    size_t m_maxRows{6};
  };

  template <typename Container> class TextField
  {
  public:
    explicit TextField(size_t lineCount = 6, _TTF_Font* font = nullptr,
               size_t fontSize = 0)
        : m_container{lineCount}
        , m_font{font}
        , m_fontSize{fontSize}
    {
    }

    ~TextField()
    {
      if (m_texture)
      {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
      }

      if (m_font)
      {
        TTF_CloseFont(m_font);
        m_font = nullptr;
      }
    }

    void add(const std::string& content)
    {
      m_container.push(content);
    }

    void clear()
    {
      m_container.clear();
    }

    bool loadFont(const std::string& filePath, size_t fontSize)
    {
      if (!filePath.empty())
      {
        if (m_font)
        {
          TTF_CloseFont(m_font);
        }
        if (m_font = TTF_OpenFont(filePath.c_str(), fontSize);
            m_font != nullptr)
        {
          m_fontSize = fontSize;
          return true;
        }
      }
      return false;
    }

    void unloadFont()
    {
      if (m_font)
      {
        TTF_CloseFont(m_font);
        m_font = nullptr;
      }
    }

    void destroy()
    {
      unloadFont();
      if (m_texture)
      {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
      }
    }

    void setFont(TTF_Font* font, size_t fontSize)
    {
      if (font)
      {
        m_font = font;
        m_fontSize = fontSize;
      }
    }

    void setPos(const gk::Vector2d& pos)
    {
      m_pos = pos;
    }

    void draw(SDL_Renderer* renderer);

  private:
    Container m_container;
    _TTF_Font* m_font{nullptr};
    SDL_Texture* m_texture{nullptr};
    Color m_textColor{Color::WHITE};
    gk::Vector2d m_pos{0, 0};
    size_t m_fontSize;
  };

  template <typename Container>
  inline void gk::TextField<Container>::draw(SDL_Renderer* renderer)
  {
    if (m_texture)
    {
      SDL_DestroyTexture(m_texture);
      m_texture = nullptr;
    }

    std::string formattedText;
    for (const auto& line : m_container.get())
    {
      formattedText += line + "\n";
    }
    const auto colorTmp = gk::Drawer::get_color_rgba(m_textColor);
    SDL_Color textColor;
    textColor.r = colorTmp.r;
    textColor.g = colorTmp.g;
    textColor.b = colorTmp.b;
    textColor.a = colorTmp.a;

    SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(
        m_font, formattedText.c_str(), textColor, 0);
    if (textSurface)
    {
      m_texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    }

    if (m_texture)
    {
      const SDL_Rect textRect = {
          m_pos.GetX<int>(), m_pos.GetY<int>(),
          static_cast<int>(textSurface->w),
          static_cast<int>(m_container.size() * m_fontSize)};
      SDL_RenderCopy(renderer, m_texture, nullptr, &textRect);
      SDL_FreeSurface(textSurface);
    }
  }
  using TextBox = TextField<TextCurcularBuffer>;
} // namespace gk
