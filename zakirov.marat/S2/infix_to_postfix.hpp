#ifndef INFIX_TO_POSTFIX_HPP
#define INFIX_TO_POSTFIX_HPP
#include <string>
#include "queue.hpp"
#include "stack.hpp"

namespace zakirov
{
  bool check_operand(const std::string & line);
  bool check_operator(std::string symbol);
  bool check_priority(std::string symbol);
  Queue< std::string > transform_to_postfix(Queue< std::string > infix);
  double transform_to_double(const std::string & line);
  double calculate_postfix(double first, double second, char oper);
  double calculate_postfix_expression(Queue< std::string > infix);
}

#endif
