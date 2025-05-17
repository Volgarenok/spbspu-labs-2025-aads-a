#include "countFunctions.h"
#include <stdexcept>
#include <cmath>

namespace
{
  bool isOperator(const std::string& s)
  {
    return s == "+" || s == "-" || s == "*" || s == "/" || s == "%";
  }
}

asafov::num_t asafov::evaluatePostfixExpression(queue_t& queue)
{
  std::stack<std::string> stack;
  while (!queue.empty())
  {
    std::string token = queue.front();
    queue.pop();
    if (isOperator(token))
    {
      if (stack.size() < 2) throw std::invalid_argument("not enough operands!");

      num_t b = std::stoll(stack.top());
      stack.pop();
      num_t a = std::stoll(stack.top());
      stack.pop();
      num_t result = 0;

      if (token == "+")
      {
        if (a > 9223372036854775807ll - b)
        {
          while (!stack.empty()) stack.pop();
          throw std::logic_error("overflow!");
        }
        result = a + b;
      }
      else if (token == "-")
      {
        if (a + 1 < -9223372036854775807ll + b)
        {
          while (!stack.empty()) stack.pop();
          throw std::logic_error("underflow!");
        }
        result = a - b;
      }
      else if (token == "*")
      {
        if (a > 0 && b > 0 && a > 9223372036854775807ll / b)
        {
          while (!stack.empty()) stack.pop();
          throw std::logic_error("overflow!");
        }
        else if (a > 0 && b < 0 && b < -9223372036854775807ll / a)
        {
          while (!stack.empty()) stack.pop();
          throw std::logic_error("underflow!");
        }
        else if (a < 0 && b > 0 && a < -9223372036854775807ll / b)
        {
          while (!stack.empty()) stack.pop();
          throw std::logic_error("underflow!");
        }
        else if (a < 0 && b < 0 && a < 9223372036854775807ll / b)
        {
          while (!stack.empty()) stack.pop();
          throw std::logic_error("overflow!");
        }
        result = a * b;
      }
      else if (token == "/")
      {
        if (b == 0.0)
        {
          while (!stack.empty()) stack.pop();
          throw std::logic_error("division by zero!");
        }
        result = a / b;
      }
      else if (token == "%")
      {
        if (b == 0)
        {
          while (!stack.empty()) stack.pop();
          throw std::logic_error("division by zero!");
        }
        result = a % b;
        if (result < 0) result += std::abs(b);
      }
      stack.push(std::to_string(result));
    }
    else
    {
      stack.push(token);
    }
  }

  if (stack.size() != 1)
  {
    while (!stack.empty()) stack.pop();
    throw std::invalid_argument("invalid expression!");
  }
  return std::stoll(stack.top());
}
