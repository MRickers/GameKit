#pragma once
#include <deque>
#include <string>
#include <vector>

#include "helpers/Draw.hpp"
#include "vector/Vector2d.hpp"

class SDL_Texture;
class _TTF_Font;

namespace gk
{
  using TextContainer = std::vector<std::string>;
  using TextBuffer = std::deque<std::string>;

  class TextCirculareBuffer
  {
  public:
    TextCirculareBuffer(size_t maxRows = 6);
    void push(const std::string& content);
    void pop();
    void clear();
    size_t size() const;
    TextContainer get() const;

  private:
    TextBuffer m_textBuffer{};
    size_t m_maxRows{6};
  };
} // namespace gk
