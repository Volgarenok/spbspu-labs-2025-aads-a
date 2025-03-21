#include "calc_post.hpp"
long long int karnauhova::calculator(long long int first, long long int second, std::string operat)
{
  if (operat == "+")
  {
    if (std::numeric_limits<long long>::max() - first < second)
    {
      throw std::logic_error("Incorrect sum");
    }
    return first + second;
  }
  if (operat == "*")
  {
    if (std::numeric_limits<long long>::max() / first < second)
    {
      throw std::logic_error("Incorrect sum");
    }
    return first * second;
  }
  if (operat == "/")
  {
    if (second == 0)
    {
      throw std::logic_error("Dividing by zero!");
    }
    return first / second;
  }
  if (operat == "%")
  {
    if (second == 0)
    {
      throw std::logic_error("Dividing by zero!");
    }
    return (first % second + second) % second;
  }
  return first - second;
}

long long int karnauhova::proc_post(karnauhova::Queue< std::string > post)
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
    if (!(element == "+" || element == "/" || element == "-" || element == "*" || element == "%"))
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
      catch(const std::exception& e)
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
          catch(const std::exception& e)
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
