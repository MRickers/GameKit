#include "GameKit/core/ui/TextBox.hpp"

gk::TextCurcularBuffer::TextCurcularBuffer(size_t maxRows)
    : m_maxRows{maxRows}
{
}

void gk::TextCurcularBuffer::push(const std::string& content)
{
  if (m_textBuffer.size() >= m_maxRows)
  {
    pop();
  }
  m_textBuffer.push_back(content);
}

void gk::TextCurcularBuffer::pop()
{
  if (!m_textBuffer.empty())
  {
    m_textBuffer.pop_front();
  }
}

void gk::TextCurcularBuffer::clear()
{
  m_textBuffer.clear();
}

size_t gk::TextCurcularBuffer::size() const
{
  return m_textBuffer.size();
}

size_t gk::TextCurcularBuffer::rows() const
{
  return m_maxRows;
}

gk::text_container gk::TextCurcularBuffer::get() const
{
  text_container container;
  for (const auto& item : m_textBuffer)
  {
    container.emplace_back(item);
  }
  return container;
}
