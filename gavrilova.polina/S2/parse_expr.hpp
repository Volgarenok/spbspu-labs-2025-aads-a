#ifndef PARSE_EXPR_HPP
#define PARSE_EXPR_HPP

#include <string>
#include <Queue.hpp>

namespace gavrilova {
  Queue< std::string > split(const std::string& line, const char& symb);
  Queue< std::string > infix_to_postfix(Queue< std::string >& infix_q);
  long long calculate(Queue< std::string >& postfix_q);
}
#endif
