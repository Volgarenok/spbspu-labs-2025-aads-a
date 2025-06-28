#include "calculate_arithmetic_expressions.hpp"
#include <limits>
#include <stdexcept>

namespace
{
  bool isAnyOperation(std::string & token)
  {
    return token == "**" || token == "/" || token == "*" || token == "%" || token == "+" || token == "-";
  }
}

petrov::Stack< long long int > petrov::calculateArithmeticExpressions(std::queue< std::string > & queue)
{
  petrov::Stack< long long int > stack;
  long long int result = 0;
  while (!queue.empty())
  {
    std::queue< std::string > new_queue = transformInfixToPostfix(queue);
    result = calculatePostfixExpression(new_queue);
    stack.push(result);
  }
  return stack;
}

std::queue< std::string > petrov::transformInfixToPostfix(std::queue< std::string > & queue)
{
  petrov::Stack< std::string > stack;
  std::queue< std::string > new_queue;
  while (queue.front() != "|")
  {
    std::string token = queue.front();
    if (token == "(")
    {
      stack.push(token);
    }
    else if (token == ")")
    {
      while (stack.top() != "(")
      {
        new_queue.push(stack.top());
        stack.pop();
      }
      stack.pop();
    }
    else if (token == "**")
    {
      if (stack.empty() || stack.top() == "**")
      {
        if (stack.empty())
        {
          stack.push(token);
        }
        else
        {
          while (!stack.empty() && stack.top() != "(")
          {
            new_queue.push(stack.top());
            stack.pop();
          }
          stack.push(token);
        }
      }
      else
      {
        stack.push(token);
      }
    }
    else if (token == "*" || token == "/" || token == "%")
    {
      if (stack.empty() || stack.top() == "**" || stack.top() == "/" || stack.top() == "*" || stack.top() == "%")
      {
        if (stack.empty())
        {
          stack.push(token);
        }
        else
        {
          while (!stack.empty() && stack.top() != "(")
          {
            new_queue.push(stack.top());
            stack.pop();
          }
          stack.push(token);
        }
      }
      else
      {
        stack.push(token);
      }
    }
    else if (token == "+" || token == "-")
    {
      if (stack.empty() || isAnyOperation(stack.top()))
      {
        if (stack.empty())
        {
          stack.push(token);
        }
        else
        {
          while (!stack.empty() && stack.top() != "(")
          {
            new_queue.push(stack.top());
            stack.pop();
          }
          stack.push(token);
        }
      }
      else
      {
        stack.push(token);
      }
    }
    else
    {
      new_queue.push(token);
    }
    queue.pop();
  }
  queue.pop();
  while (!stack.empty())
  {
    if (stack.top() == "(" || stack.top() == ")")
    {
      throw std::invalid_argument("ERROR: Incorrect Expression");
    }
    new_queue.push(stack.top());
    stack.pop();
  }
  return new_queue;
}

long long int petrov::calculatePostfixExpression(std::queue< std::string > & queue)
{
  petrov::Stack< long long int > stack;
  long long int result = 0;
  try
  {
    while (!queue.empty())
    {
      std::string token = queue.front();
      if (token == "**")
      {
        long long int first_operand = stack.top();
        stack.pop();
        long long int second_operand = stack.top();
        stack.pop();
        long long int multiplier = second_operand;
        if (first_operand == 0)
        {
          stack.push(1ll);
        }
        else
        {
          for (long long int i = 1; i < first_operand; i++)
          {
            if (std::abs(second_operand) <= std::numeric_limits< long long int >::max() / std::abs(multiplier))
            {
              second_operand *= multiplier;
            }
            else
            {
              throw std::out_of_range("ERROR: Out of range");
            }
          }
          result = second_operand;
          stack.push(result);
        }
      }
      else if (token == "*")
      {
        long long int first_operand = stack.top();
        stack.pop();
        long long int second_operand = stack.top();
        stack.pop();
        if (std::abs(second_operand) <= std::numeric_limits< long long int >::max() / std::abs(first_operand))
        {
          result = second_operand * first_operand;
        }
        else
        {
          throw std::out_of_range("ERROR: Out of range");
        }
        stack.push(result);
      }
      else if (token == "/")
      {
        long long int first_operand = stack.top();
        stack.pop();
        long long int second_operand = stack.top();
        stack.pop();
        result = second_operand / first_operand;
        stack.push(result);
      }
      else if (token == "%")
      {
        long long int first_operand = stack.top();
        stack.pop();
        long long int second_operand = stack.top();
        stack.pop();
        if (first_operand < 0)
        {
          result = second_operand % std::abs(first_operand);
          result = first_operand - result;
        }
        else if (second_operand < 0)
        {
          result = std::abs(second_operand) % first_operand;
          result = first_operand - result;
        }
        else
        {
          result = second_operand % first_operand;
        }
        stack.push(result);
      }
      else if (token == "+")
      {
        long long int first_operand = stack.top();
        stack.pop();
        long long int second_operand = stack.top();
        stack.pop();
        if (second_operand <= std::numeric_limits< long long int >::max() - first_operand)
        {
          result = second_operand + first_operand;
        }
        else
        {
          throw std::out_of_range("ERROR: Out of range");
        }
        stack.push(result);
      }
      else if (token == "-")
      {
        long long int first_operand = stack.top();
        stack.pop();
        long long int second_operand = stack.top();
        stack.pop();
        if (second_operand >= std::numeric_limits< long long int >::min() + first_operand)
        {
          result = second_operand - first_operand;
        }
        else
        {
          throw std::out_of_range("ERROR: Out of range");
        }
        stack.push(result);
      }
      else
      {
        stack.push(std::stoll(token));
      }
      queue.pop();
    }
  }
  catch (const std::out_of_range & e)
  {
    throw;
  }
  return stack.top();
}
