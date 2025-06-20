#include "calcExprs.hpp"
#include <stdexcept>
#include <sstream>
#include <limits>
#include "stack.hpp"

namespace
{
  constexpr long long int max = std::numeric_limits< long long int >::max();
  constexpr long long int min = std::numeric_limits< long long int >::min();

  int sign(long long int a)
  {
    if (a > 0)
    {
      return 1;
    }
    else if (a < 0)
    {
      return -1;
    }
    else
    {
      return 0;
    }
  }

  bool same_sign(long long int a, long long int b)
  {
    return sign(a) * sign(b) > 0;
  }

  long long int sum(long long int a, long long int b)
  {
    if (same_sign(a, b) && a > 0 && max - a >= b)
    {
      return a + b;
    }
    else if (same_sign(a, b) && a < 0 && min - a <= b)
    {
      return a + b;
    }
    else if (a == 0 || b == 0)
    {
      return a + b;
    }
    else if (!same_sign(a, b))
    {
      return a + b;
    }
    throw std::logic_error("Overflow\n");
  }

  long long int diff(long long int a, long long int b)
  {
    if (same_sign(a, b) && a > 0 && min + b <= a)
    {
      return a - b;
    }
    else if (same_sign(a, b) && a < 0 && max - a >= b)
    {
      return a - b;
    }
    else if (a == 0 || b == 0)
    {
      return a - b;
    }
    else if (!same_sign(a, b))
    {
      return a - b;
    }
    throw std::logic_error("Overflow\n");
  }

  long long int prod(long long int a, long long int b)
  {
    if (same_sign(a, b) && a > 0 && max / a >= b)
    {
      return a * b;
    }
    else if (same_sign(a, b) && a < 0 && max / a >= std::abs(b))
    {
      return a * b;
    }
    else if (a == 0 || b == 0)
    {
      return 0ll;
    }
    else if (!same_sign(a, b) && std::abs(min / a) >= std::abs(b))
    {
      return a * b;
    }
    throw std::logic_error("Overflow\n");
  }

  long long int quot(long long int a, long long int b)
  {
    if (b == 0)
    {
      throw std::logic_error("Division by zero\n");
    }
    else if (a == 0)
    {
      return 0ll;
    }
    else if (same_sign(a, b) && a < 0 && a != min && b != -1ll)
    {
      return a / b;
    }
    else
    {
      return a / b;
    }
    throw std::logic_error("Overflow\n");
  }

  long long int doOperation(long long int a, long long int b, const std::string &op)
  {
    if (op == "+")
    {
      return sum(a, b);
    }
    else if (op == "-")
    {
      return diff(a, b);
    }
    else if (op == "*")
    {
      return prod(a, b);
    }
    else if (op == "/")
    {
      return quot(a, b);
    }
    else if (op == "%")
    {
      if (a < 0)
      {
        return b - std::abs(a) % b;
      }
      else if (b == 0)
      {
        throw std::logic_error("Division by zero\n");
      }
      else
      {
        return a % b;
      }
    }
    throw std::logic_error("Operation not supported\n");
  }

  bool isNumber(const std::string &s)
  {
    try
    {
      std::stoll(s);
    }
    catch (...)
    {
      return false;
    }
    for (auto it = s.begin(); it != s.end(); ++it)
    {
      if (!std::isdigit(*it))
      {
        return false;
      }
    }
    return true;
  }
}

long long int abramov::calcExpr(const std::string &s)
{
  Queue< std::string > *postfix = getPostfix(s);
  long long int res = 0ll;
  try
  {
    res = calcPostfix(postfix);
  }
  catch (...)
  {
    delete postfix;
    throw;
  }
  delete postfix;
  return res;
}

long long int abramov::calcPostfix(abramov::Queue< std::string > *queue)
{
  Stack< long long int > stack;
  std::string temp;
  while(!queue->empty())
  {
    temp = queue->front();
    queue->pop();
    if (isNumber(temp))
    {
      stack.push(std::stoll(temp));
    }
    else
    {
      long long int num_r = stack.top();
      stack.pop();
      long long int num_l = stack.top();
      stack.pop();
      stack.push(doOperation(num_l, num_r, temp));
    }
  }
  if (stack.size() != 1)
  {
    throw std::logic_error("Wrong expression\n");
  }
  return stack.top();
}

abramov::Queue< std::string > *abramov::getPostfix(const std::string &s)
{
  Queue< std::string > queue_infix;
  std::stringstream ss(s);
  std::string token;
  while (ss >> token)
  {
    queue_infix.push(token);
  }
  Stack< std::string > stack;
  Queue< std::string > *queue_postfix = new Queue< std::string >;
  bool bracket = false;
  while(!queue_infix.empty())
  {
    std::string temp = queue_infix.front();
    queue_infix.pop();
    if (temp == "(")
    {
      stack.push("(");
      bracket = true;
    }
    else if (temp == ")")
    {
      if (!bracket)
      {
        delete queue_postfix;
        throw std::logic_error("Unbalanced brackets\n");
      }
      while (stack.top() != "(")
      {
        queue_postfix->push(stack.top());
        stack.pop();
      }
      stack.pop();
      bracket = false;
    }
    else if (isNumber(temp))
    {
      queue_postfix->push(temp);
    }
    else if (temp == "+" || temp == "-")
    {
      if (!stack.empty())
      {
        if (stack.top() != "(")
        {
          queue_postfix->push(stack.top());
          stack.pop();
        }
      }
      stack.push(temp);
    }
    else if (temp == "*" || temp == "/" || temp == "%")
    {
      if (!stack.empty())
      {
        if (stack.top() == "*" || stack.top() == "/" || stack.top() == "%")
        {
          queue_postfix->push(stack.top());
          stack.pop();
        }
      }
      stack.push(temp);
    }
    else
    {
      delete queue_postfix;
      throw std::logic_error("Not supported symbol\n");
    }
  }
  while (!stack.empty())
  {
    queue_postfix->push(stack.top());
    stack.pop();
  }
  if (bracket)
  {
    delete queue_postfix;
    throw std::logic_error("Unbalanced brackets\n");
  }
  return queue_postfix;
}
