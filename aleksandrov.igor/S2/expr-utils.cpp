#include "expr-utils.hpp"
#include <limits>
#include <stdexcept>
#include "queue.hpp"
#include "stack.hpp"

namespace aleksandrov
{
  namespace detail
  {
    bool isOperation(const std::string& str)
    {
      return str == "+" || str == "-" || str == "*" || str == "/" || str == "%";
    }

    bool isNumber(const std::string& str)
    {
      for (char c : str)
      {
        if (!std::isdigit(c))
        {
          return false;
        }
      }
      return true;
    }

    bool isLessImportant(const std::string& op1, const std::string& op2)
    {
      if (op1 == "+" || op1 == "-")
      {
        return op2 == "*" || op2 == "/" || op2 == "%";
      }
      return false;
    }

    int sign(long long int val)
    {
      return (val > 0) ? 1 : ((val < 0) ? -1 : 0);
    }

    bool sameSign(long long int a, long long int b)
    {
      return sign(a) * sign(b) > 0;
    }
  }

  using namespace detail;

  long long int sum(long long int a, long long int b)
  {
    const long long int max = std::numeric_limits< long long int >::max();
    const long long int min = std::numeric_limits< long long int >::min();

    if ((!a && !b) || a == -b)
    {
      return 0ll;
    }

    long long int result = 0ll;
    if (sameSign(a, b) && a > 0)
    {
      if (max - a >= b)
      {
        result = a + b;
      }
    }
    else if (sameSign(a, b) && a < 0)
    {
      if (min - a <= b)
      {
        result = a + b;
      }
    }
    else if (!sameSign(a, b))
    {
      result = a + b;
    }

    if (!result)
    {
      throw std::overflow_error("There was an overflow error!");
    }
    return result;
  }

  long long int mult(long long int a, long long int b)
  {
    const long long int max = std::numeric_limits< long long int >::max();
    const long long int min = std::numeric_limits< long long int >::min();

    if (!a || !b)
    {
      return 0ll;
    }

    if (sameSign(a, b))
    {
      if (a > 0ll)
      {
        if (a > max / b)
        {
          throw std::overflow_error("There was an overflow error!");
        }
      }
      else
      {
        if (a < min / b)
        {
          throw std::overflow_error("There was an overflow error!");
        }
      }
    }
    else
    {
      if (a > 0ll)
      {
        if (b < min / a)
        {
          throw std::overflow_error("There was an overflow error!");
        }
      }
      else
      {
        if (a < min / b)
        {
          throw std::overflow_error("There was an overflow error!");
        }
      }
    }
    return a * b;
  }

  long long int div(long long int a, long long int b)
  {
    if (!b)
    {
      throw std::invalid_argument("There was division by zero!");
    }

    const long long int min = std::numeric_limits< long long int >::min();

    if (a < b && -a < b)
    {
      return 0ll;
    }

    long long int result = 0ll;
    if (a != min || b != -1)
    {
      result = a / b;
    }
    if (!result)
    {
      throw std::overflow_error("There was an overflow error!");
    }
    return result;
  }

  long long int mod(long long int a, long long int b)
  {
    if (!b)
    {
      throw std::invalid_argument("There was division by zero!");
    }
    return (a % b + std::abs(b)) % std::abs(b);
  }

  Queue< std::string > infixToPostfix(Queue< std::string>& infix)
  {
    Queue< std::string > postfix;
    Stack< std::string > stack;
    while (!infix.empty())
    {
      std::string token = infix.drop();
      if (isNumber(token))
      {
        postfix.push(token);
      }
      else if (token == "(")
      {
        stack.push(token);
      }
      else if (token == ")")
      {
        while (!stack.empty() && stack.top() != "(")
        {
          postfix.push(stack.drop());
        }
        if (stack.empty())
        {
          throw std::logic_error("Mismatched parentheses!");
        }
        stack.drop();
      }
      else if (isOperation(token))
      {
        while (!stack.empty() && isOperation(stack.top()) && !isLessImportant(stack.top(), \
token))
        {
          postfix.push(stack.drop());
        }
        stack.push(token);
      }
      else
      {
        throw std::logic_error("Incorrect expression!");
      }
    }
    while (!stack.empty())
    {
      if (stack.top() == "(" || stack.top() == ")")
      {
        throw std::logic_error("Mismatched parentheses!");
      }
      postfix.push(stack.drop());
    }
    return postfix;
  }

  long long int evalPostfix(Queue< std::string >& postfix)
  {
    Stack< long long int > stack;
    if (postfix.size() == 1)
    {
      std::string token = postfix.drop();
      if (isNumber(token))
      {
        return std::stoll(token);
      }
    }
    while (!postfix.empty())
    {
      std::string token = postfix.drop();
      if (isNumber(token))
      {
        stack.push(std::stoll(token));
      }
      else if (isOperation(token))
      {
        if (stack.size() < 2)
        {
          throw std::logic_error("Incorrect expression!");
        }
        long long int second = stack.drop();
        long long int first = stack.drop();
        long long int result = 0ll;
        if (token == "+")
        {
          result = sum(first, second);
        }
        else if (token == "-")
        {
          result = sum(first, -second);
        }
        else if (token == "*")
        {
          result = mult(first, second);
        }
        else if (token == "/")
        {
          result = div(first, second);
        }
        else if (token == "%")
        {
          result = mod(first, second);
        }
        else
        {
          throw std::logic_error("Incorrect expression!");
        }
        stack.push(result);
      }
    }
    if (stack.size() != 1)
    {
      throw std::logic_error("Incorrect expression!");
    }
    return stack.top();
  }
}

