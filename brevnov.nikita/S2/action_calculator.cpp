#include "action_calculator.hpp"
#include <limits>
#include <stdexcept>

namespace
{
  int get_priority(const std::string& operation)
  {
    if (operation == "+" or operation == "-")
    {
      return 1;
    }
    else if (operation == "*" or operation == "/" or operation == "%")
    {
      return 2;
    }
    return 0;
  }

  bool is_operator(const std::string& a)
  {
    return a == "+" || a == "-" || a == "*" || a == "/" || a == "%";
  }

  void check_multiplicate(long long int a, long long int b)
  {
    long long int max = std::numeric_limits< long long int >::max();
    long long int min = std::numeric_limits< long long int >::min();
    bool is_Overflow = false;
    if (a > 0 && b > 0 && a > max / b)
    {
      is_Overflow = true;
    }
    else if (a > 0 && b < 0 && b < min / a)
    {
      is_Overflow = true;
    }
    else if (a < 0 && b > 0 && a < min / b)
    {
      is_Overflow = true;
    }
    else if (a < 0 && b < 0 && b < max / a)
    {
      is_Overflow = true;
    }
    if (is_Overflow)
    {
      throw std::logic_error("Overflow");
    }
  }

  void check_sum(long long int a, long long int b)
  {
    long long int max = std::numeric_limits< long long int >::max();
    if (a > max - b)
    {
      throw std::logic_error("Overflow");
    }
  }

  void check_difference(long long int a, long long b)
  {
    long long int min = std::numeric_limits< long long int >::min();
    if (a < min + b)
    {
      throw std::logic_error("Underflow");
    }
  }

  void check_division(long long int b)
  {
    if (b == 0)
    {
      throw std::logic_error("Dividing by 0");
    }
  }

  long long int calculating(long long int a, long long b, const std::string& operation)
  {
    switch (operation.front())
    {
    case '+':
      check_sum(a, b);
      return a + b;
    case '-':
      check_difference(a, b);
      return a - b;
    case '*':
      check_multiplicate(a, b);
      return a * b;
    case '/':
      check_division(b);
      return a / b;
    case '%':
      check_division(b);
      return a >= 0 ? a % b : (b - std::abs(a % b)) % b;
    default:
      throw std::logic_error("Invalid operation");
    }
  }
}

brevnov::queue brevnov::convert_expression(queue& infix)
{
  queue postfix_expressions;
  while (!infix.empty())
  {
    expression infix_expression = infix.front();
    infix.pop();
    expression postfix_expression;
    Stack< std::string > operators;
    while (!infix_expression.empty())
    {
      std::string part = infix_expression.front();
      infix_expression.pop();
      if (part == "(")
      {
        operators.push(part);
      }
      else if (part == ")")
      {
        while (!operators.empty() && operators.top() != "(")
        {
          postfix_expression.push(operators.top());
          operators.pop();
        }
        operators.pop();
      }
      else if (is_operator(part))
      {
        while (!operators.empty() && get_priority(operators.top()) >= get_priority(part))
        {
          postfix_expression.push(operators.top());
          operators.pop();
        }
        operators.push(part);
      }
      else
      {
        postfix_expression.push(part);
      }
    }
    while (!operators.empty())
    {
      postfix_expression.push(operators.top());
      operators.pop();
    }
    postfix_expressions.push(postfix_expression);
  }
  return postfix_expressions;
}

brevnov::stack_number brevnov::calculation_expression(queue& postfix_expressions)
{
  stack_number results;
  while (!postfix_expressions.empty())
  {
    stack_number numbers;
    expression postfix_expression = postfix_expressions.front();
    postfix_expressions.pop();
    while (!postfix_expression.empty())
    {
      std::string part = postfix_expression.front();
      postfix_expression.pop();
      if (is_operator(part))
      {
        if (numbers.size() < 2)
        {
          throw std::logic_error("Not enough operands");
        }
        long long int right = numbers.top();
        numbers.pop();
        long long int left = numbers.top();
        numbers.pop();
        long long int result = calculating(left, right, part);
        numbers.push(result);
      }
      else
      {
        try
        {
          long long int number = std::stoll(part);
          numbers.push(number);
        }
        catch (...)
        {
          throw;
        }
      }
    }
    if (numbers.size() != 1)
    {
      throw std::logic_error("Too many operands");
    }
    results.push(numbers.top());
  }

  return results;
}

void brevnov::input_expression(std::istream& input, queue& expressions)
{
  std::string line;
  while (std::getline(input, line))
  {
    if (line.empty())
    {
      continue;
    }
    expression infix_expression;
    std::string part;
    size_t start = 0;
    size_t end = line.find(' ');
    while (end != std::string::npos)
    {
      part = line.substr(start, end - start);
      if (!part.empty())
      {
        infix_expression.push(part);
      }
      start = end + 1;
      end = line.find(' ', start);
    }
    part = line.substr(start);
    if (!part.empty())
    {
      infix_expression.push(part);
    }
    expressions.push(infix_expression);
  }
}

std::ostream& brevnov::output_results(std::ostream& output, stack_number& results)
{
  if (!results.empty())
  {
    output << results.top();
    results.pop();
    while (!results.empty())
    {
      output << " " << results.top();
      results.pop();
    }
  }
  return output;
}
