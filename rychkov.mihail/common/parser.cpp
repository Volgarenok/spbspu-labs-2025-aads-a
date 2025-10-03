#include "parser.hpp"

#include <iostream>
#include <cctype>

bool rychkov::eol(std::istream& in)
{
  for (char c = in.peek(); in && (in.peek() != '\n'); in.get(), c = in.peek())
  {
    if (!std::isspace(c))
    {
      return false;
    }
  }
  return true;
}
void rychkov::ParserContext::parse_error()
{
  out << "<INVALID COMMAND>\n";
  in.clear(in.rdstate() & ~std::ios::failbit);
  in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
}
