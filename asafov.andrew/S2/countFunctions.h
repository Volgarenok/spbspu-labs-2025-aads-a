#ifndef PARSING_FUNCTIONS_H
#define PARSING_FUNCTIONS_H

#include <string>
#include <stack>
#include <queue>
#include <iostream>

namespace asafov
{
  using num_t = long long;
  using stack_t = std::stack<std::string>;
  using queue_t = std::queue<std::string>;

  void parseStringToQueue(queue_t& queue, const std::string& string);
  void convertInfixToPostfix(queue_t& queue);
  num_t evaluatePostfixExpression(queue_t& queue);
  int evaluateExpressionFromString(std::istream& is, std::stack<num_t>& result);
}
#endif
