#ifndef IO_INFIX_HPP
#define IO_INFIX_HPP
#include <iostream>
#include "postfix.hpp"

namespace sveshnikov
{
  sveshnikov::Queue< sveshnikov::Postfix > inputInfix(std::istream &in);
  sveshnikov::Queue< std::string > splitIntoTokens(const std::string &expr);
}

#endif
