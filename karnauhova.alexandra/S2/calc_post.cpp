#include "calc_post.hpp"
#include <limits>
#include "input.hpp"

namespace
{
  void isNull(long long int number)
  {
    if (number == 0)
    {
      throw std::logic_error("Dividing by zero!");
    }
  }
}

long long int karnauhova::getSum(long long int first, long long int second)
{
  if (std::numeric_limits< long long >::max() - first < second)
  {
    throw std::logic_error("Incorrect sum");
  }
  return first + second;
}

long long int karnauhova::getDifference(long long int first, long long int second)
{
  return first - second;
}

long long int karnauhova::getMultiplicate(long long int first, long long int second)
{
  if (std::numeric_limits< long long >::max() / first < second)
  {
    throw std::logic_error("Incorrect sum");
  }
  return first * second;
}

long long int karnauhova::getDivisionMain(long long int first, long long int second)
{
  isNull(second);
  return first / second;
}

long long int karnauhova::getDivisionRemain(long long int first, long long int second)
{
  isNull(second);
  return (first % second + second) % second;
}

long long int karnauhova::calculator(long long int first, long long int second, std::string operat)
{
  switch (operat.front())
  {
    case '+':
      return getSum(first, second);
    case '-':
      return getDifference(first, second);
    case '*':
      return getMultiplicate(first, second);
    case '/':
      return getDivisionMain(first, second);
    case '%':
      return getDivisionRemain(first, second);
    default:
      throw std::logic_error("Invalid operation");
  }
}

long long int karnauhova::procPost(Queue< std::string > post)
{
  long long int sum = 0;
  long long int first = 0;
  long long int second = 0;
  while (!post.empty())
  {

    std::string element = post.front();
    first = std::stoll(element);
    post.pop();
    if (post.empty() && sum == 0)
    {
      return first;
    }
    else if (post.empty())
    {
      throw std::logic_error("Incorrect");
    }
    element = post.front();
    if (!(isOperator(element)))
    {
      second = std::stoll(element);
      post.pop();
      element = post.front();
      try
      {
        std::stoll(element);
        post.pop();
        std::string oper = post.front();
        post.pop();
        second = calculator(second, std::stoll(element), oper);
        element = post.front();
        sum += calculator(first, second, element);
        post.pop();
      }
      catch (const std::exception& e)
      {
        post.pop();
        if (!post.empty())
        {
          std::string it = post.front();
          try
          {
            std::stoll(it);
            sum += calculator(first, second, element);
          }
          catch (const std::exception& e)
          {
            post.pop();
            second = calculator(first, second, element);
            sum = calculator(sum, second, it);
          }
        }
        else
        {
          sum += calculator(first, second, element);
        }
      }
    }
    else
    {
      sum = calculator(sum, first, element);
      post.pop();
    }
  }
  return sum;
}
