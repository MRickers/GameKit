#include "GameKit/core/ui/text_box.hpp"

gk::text_curcular_buffer::text_curcular_buffer(size_t maxRows)
    : m_maxRows{maxRows}
{
}

void gk::text_curcular_buffer::push(const std::string& content)
{
  if (m_textBuffer.size() >= m_maxRows)
  {
    pop();
  }
  m_textBuffer.push_back(content);
}

void gk::text_curcular_buffer::pop()
{
  if (!m_textBuffer.empty())
  {
    m_textBuffer.pop_front();
  }
}

void gk::text_curcular_buffer::clear()
{
  m_textBuffer.clear();
}

size_t gk::text_curcular_buffer::size() const
{
  return m_textBuffer.size();
}

size_t gk::text_curcular_buffer::rows() const
{
  return m_maxRows;
}

gk::text_container gk::text_curcular_buffer::get() const
{
  text_container container;
  for (const auto& item : m_textBuffer)
  {
    container.emplace_back(item);
  }
  return container;
}
