#ifndef INFIX_TO_POSTFIX_HPP
#define INFIX_TO_POSTFIX_HPP
#include <string>
#include "queue.hpp"
#include "stack.hpp"

namespace zakirov
{
  using llint = long long int;
  bool check_operand(const std::string & line);
  bool check_operator(std::string symbol);
  bool check_priority(std::string symbol);
  void check_overflow(llint first, llint second, char oper);
  Queue< std::string > transform_to_postfix(Queue< std::string > infix);
  llint transform_to_llint(const std::string & line);
  llint calculate_postfix(llint first, llint second, char oper);
  llint calculate_postfix_expression(Queue< std::string > infix);
}

#endif
