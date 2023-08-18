#include "TextBox.hpp"

gk::TextCirculareBuffer::TextCirculareBuffer(size_t maxRows)
    : m_maxRows{maxRows}
{
}

void gk::TextCirculareBuffer::push(const std::string& content)
{
  if (m_textBuffer.size() >= m_maxRows)
  {
    pop();
  }
  m_textBuffer.push_back(content);
}

void gk::TextCirculareBuffer::pop()
{
  if (!m_textBuffer.empty())
  {
    m_textBuffer.pop_front();
  }
}

void gk::TextCirculareBuffer::clear()
{
  m_textBuffer.clear();
}

size_t gk::TextCirculareBuffer::size() const
{
  return m_textBuffer.size();
}

size_t gk::TextCirculareBuffer::rows() const
{
  return m_maxRows;
}

gk::TextContainer gk::TextCirculareBuffer::get() const
{
  TextContainer container;
  for (const auto& item : m_textBuffer)
  {
    container.emplace_back(item);
  }
  return container;
}
