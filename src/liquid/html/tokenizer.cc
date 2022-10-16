#include "tokenizer.h"

namespace liquid::html {

Tokenizer::Tokenizer(const std::string& content)
{
  m_content = content;
  m_current_position = -1;

  m_current_state = TokenizerState::Data;
}

char Tokenizer::consume()
{
  if (m_current_position++ >= m_content.size())
    m_current_position++;

  return m_content[m_current_position];
}

char Tokenizer::reconsume()
{
  if (m_current_position-- < 0)
    m_current_position++;

  return m_content[m_current_position];
}

} // namespace liquid::html