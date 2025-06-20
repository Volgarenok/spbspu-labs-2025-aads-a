#include "countFunctions.h"
#include <stdexcept>

namespace
{
  bool is_operator(const std::string& s)
  {
    return s == "+" || s == "-" || s == "*" || s == "/" || s == "%";
  }

  int get_priority(const std::string& op)
  {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/" || op == "%") return 2;
    return 0;
  }
}

void asafov::convertInfixToPostfix(queue_str_t& queue)
{
  queue_str_t output;
  stack_str_t ops;

  while (!queue.empty())
  {
    std::string token = queue.front();
    queue.pop();

    if (token == "(")
    {
      ops.push(token);
    }
    else if (token == ")")
    {
      while (!ops.empty() && ops.top() != "(")
      {
        output.push(ops.top());
        ops.pop();
      }

      if (!ops.empty())
      {
        ops.pop();
      }
      else
      {
        throw std::logic_error("mismatched parentheses!");
      }
    }
    else if (is_operator(token))
    {
      while (!ops.empty() && ops.top() != "(" && get_priority(ops.top()) >= get_priority(token))
      {
        output.push(ops.top());
        ops.pop();
      }
      ops.push(token);
    }
    else
    {
      output.push(token);
    }
  }

  while (!ops.empty())
  {
    if (ops.top() == "(")
    {
      throw std::logic_error("mismatched parentheses!");
    }
    output.push(ops.top());
    ops.pop();
  }

  queue = std::move(output);
}
