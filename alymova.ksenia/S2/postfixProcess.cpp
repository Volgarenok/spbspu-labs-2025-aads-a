#include "postfixProcess.hpp"
#include <cstddef>
#include <string>
#include <sstream>
#include <limits>
#include <stdexcept>
#include <iostream>
#include "queue.hpp"
#include "stack.hpp"

alymova::Queue< std::string > alymova::convert_postfix(const std::string& s)
{
  Stack< std::string > stack;
  Queue< std::string > queue;

  std::string s_local = s;
  while (!s_local.empty())
  {
    size_t space = s_local.find(" ");
    std::string now;
    if (space == std::string::npos)
    {
      now = s_local;
      space = s_local.size() - 1;
    }
    else
    {
      now = s_local.substr(0, space);
    }
    s_local = s_local.substr(space + 1);

    if (now == "(")
    {
      stack.push(now);
    }
    else if (now == ")")
    {
      while (stack.top() != "(")
      {
        queue.push(stack.top());
        stack.pop();
        if (stack.empty())
        {
          throw std::logic_error("Incorrect expression");
        }
      }
      stack.pop();
    }
    else if (my_isdigit(now))
    {
      queue.push(now);
    }
    else if (is_first_priority(now) || is_second_priority(now))
    {
      if (!stack.empty())
      {
        while (is_need_priority(now, stack.top()))
        {
          queue.push(stack.top());
          stack.pop();
          if (stack.empty())
          {
            break;
          }
        }
      }
      stack.push(now);
    }
    else
    {
      throw std::logic_error("Incorrect char");
    }
  }
  while (!stack.empty())
  {
    if (stack.top() == "(")
    {
      throw std::logic_error("Incorrect expression");
    }
    queue.push(stack.top());
    stack.pop();
  }
  return queue;
}
long long int alymova::count_postfix(alymova::Queue< std::string >& queue)
{
  Stack< long long int > stack; //exp
  while (!queue.empty())
  {
    if (my_isdigit(queue.front()))
    {
      stack.push(std::stoll(queue.front())); //exp
    }
    else
    {
      if (stack.size() < 2)
      {
        throw std::logic_error("Incorrect expression");
      }
      long long int item2 = stack.top();
      stack.pop();
      long long int item1 = stack.top();
      stack.pop();
      switch ((queue.front())[0])
      {
      case '+':
        if (is_overflow_addition(item1, item2))
        {
          throw std::overflow_error("Addition overflow");
        }
        stack.push(item1 + item2);
        break;
      case '-':
        if (is_overflow_substraction(item1, item2))
        {
          std::cout << item1 << " " << item2 << '\n';
          throw std::overflow_error("Substraction overflow");
        }
        stack.push(item1 - item2);
        break;
      case '*':
        if (is_overflow_multi(item1, item2))
        {
          throw std::overflow_error("Multiplication overflow");
        }
        stack.push(item1 * item2);
        break;
      case '/':
        if (item2 == 0)
        {
          throw std::logic_error("Division by 0");
        }
        stack.push(item1 / item2);
        break;
      case '%':
        if (item2 == 0)
        {
          throw std::logic_error("Division by 0");
        }
        stack.push(my_mod(item1, item2));
        break;
      }
    }
    queue.pop();
  }
  return stack.top();
}
bool alymova::my_isdigit(const std::string& s)
{
  size_t i = 0;
  if (s[0] == '-' && s.size() > 1)
  {
    i++;
  }
  for (; i < s.size(); i++)
  {
    if (!std::isdigit(s[i]))
    {
      return false;
    }
  }
  return true;
}
bool alymova::is_overflow_addition(long long int lhs, long long int rhs)
{
  long long int max_sum = std::numeric_limits< long long int >::max();
  long long int min_sum = std::numeric_limits< long long int >::min();
  if (lhs > 0 && rhs > 0)
  {
    return ((max_sum - rhs) < lhs);
  }
  if (lhs < 0 && rhs < 0)
  {
    return ((min_sum - rhs) > lhs);
  }
  return false;
}
bool alymova::is_overflow_substraction(long long int lhs, long long int rhs)
{
  long long int max_sum = std::numeric_limits< long long int >::max();
  long long int min_sum = std::numeric_limits< long long int >::min();
  if (lhs > 0 && rhs < 0)
  {
    return ((max_sum + rhs) < lhs);
  }
  if (lhs < 0 && rhs > 0)
  {
    return ((min_sum + rhs) > lhs);
  }
  return false;
}
bool alymova::is_overflow_multi(long long int lhs, long long int rhs)
{
  long long int max_sum = std::numeric_limits< long long int >::max();
  long long int min_sum = std::numeric_limits< long long int >::min();
  if (same_sign(lhs, rhs))
  {
    return (std::abs(max_sum / lhs) < std::abs(rhs));
  }
  else
  {
    return (std::abs(min_sum / lhs) < std::abs(rhs));
  }
  return false;
}
long long int alymova::my_mod(long long int item1, long long int item2)
{
  long long int quot = item1 / item2;
  if (item1 < 0 && item2 < 0)
  {
    quot++;
  }
  if (item1 < 0 && item2 > 0)
  {
    quot--;
  }
  long long int res = item1 - quot * item2;
  return res;
}
int alymova::sign(long long int item)
{
  return (item == 0ull ? 0 : (item < 0 ? (-1) : 1));
}
bool alymova::same_sign(long long int item1, long long int item2)
{
  return (sign(item1) * sign(item2) > 0);
}
bool alymova::is_first_priority(const std::string& token)
{
  return (token == "*" || token == "/" || token == "%");
}
bool alymova::is_second_priority(const std::string& token)
{
  return (token == "+" || token == "-");
}
bool alymova::is_need_priority(const std::string& item, const std::string& token)
{
  if (is_second_priority(item))
  {
    return (is_first_priority(token) || is_second_priority(token));
  }
  else
  {
    return is_first_priority(token);
  }
}
