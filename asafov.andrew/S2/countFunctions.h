#ifndef PARSING_FUNCTIONS_H
#define PARSING_FUNCTIONS_H

#include <string>
#include <iostream>
#include <stack.hpp>
#include <queue.hpp>

namespace asafov
{
  using num_t = long long;
  using stack_str_t = asafov::Stack< std::string >;
  using stack_num_t = asafov::Stack< num_t >;
  using queue_str_t = asafov::Queue< std::string >;

  void parseStringToQueue(queue_str_t& queue, const std::string& string);
  void convertInfixToPostfix(queue_str_t& queue);
  num_t evaluatePostfixExpression(queue_str_t& queue);
  int evaluateExpressionFromString(std::istream& is, asafov::stack_num_t& result);
  void count(asafov::stack_num_t& result, std::istream& is);
}
#endif
