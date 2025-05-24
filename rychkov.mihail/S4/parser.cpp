#include "parser.hpp"

#include <cctype>

bool rychkov::ParserContext::eol()
{
  for (char c = in.peek(); in.peek() != '\n'; in.get(), c = in.peek())
  {
    if (!std::isspace(c))
    {
      return false;
    }
  }
  return true;
}
