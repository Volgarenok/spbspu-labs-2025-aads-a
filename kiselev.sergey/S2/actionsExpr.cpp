#include "actionsExpr.hpp"
#include <limits>
#include <cctype>
#include <new>
#include <ostream>
#include <stdexcept>
#include <string>

namespace
{
  int getPriority(const char& operation)
  {
    if (operation == '+' || operation == '-')
    {
      return 1;
    }
    else if (operation == '*' || operation == '/' || operation == '%')
    {
      return 2;
    }
    return 0;
  }

  bool isOperator(const char& c)
  {
    return c == '+' || c == '-' || c == '/' || c == '*' || c == '%';
  }
  void addNumberInLine(std::string& line, std::string& number)
  {
    if (!number.empty())
    {
      if (!line.empty())
      {
        line += ' ';
      }
      line += number;
      number.clear();
    }
  }

  void addNumberInStack(std::string& number, kiselev::stackNumber& stack)
  {
    if (!number.empty())
    {
      stack.push(std::stoll(number));
      number.clear();
    }
  }

  bool isMultiplicateOverflow(long long int a, long long int b)
  {
    long long int max = std::numeric_limits< long long int >::max();
    long long int min = std::numeric_limits< long long int >::min();
    if (a > 0 && b > 0 && a > max / b)
    {
      return true;
    }
    else if (a > 0 && b < 0 && b < min / a)
    {
      return true;
    }
    else if (a < 0 && b > 0 && a < min / b)
    {
      return true;
    }
    else if (a < 0 && b < 0 && b < max / a)
    {
      return true;
    }
    return false;
  }
  long long int calculatingOperation(long long int a, long long b, const char& operation)
  {
    long long int max = std::numeric_limits< long long int >::max();
    long long int min = std::numeric_limits< long long int >::min();
    switch (operation)
    {
    case '+':
      if (a > max - b)
      {
        throw std::logic_error("Overflow");
      }
      return a + b;
    case '-':
      if (a < min + b)
      {
        throw std::logic_error("Underflow");
      }
      return a - b;
    case '*':
      if (isMultiplicateOverflow(a, b))
      {
        throw std::logic_error("Overflow");
      }
      return a * b;
    case '/':
      if (b == 0)
      {
        throw std::logic_error("Dividing by 0");
      }
      return a / b;
    case '%':
      if (b == 0)
      {
        throw std::logic_error("Dividing by 0");
      }
      return a >= 0 ? a % b : (b - std::abs(a % b)) % b;
    default:
      throw std::logic_error("Invalid operation");
    }
  }

}

kiselev::queue kiselev::convertExpr(queue& infix)
{
  queue postfix;
  Stack< char > operators;
  std::string number;
  while (!infix.empty())
  {
    std::string line = infix.front();
    infix.pop();
    std::string postfixLine;
    for (char s: line)
    {
      if (isspace(s))
      {
        addNumberInLine(postfixLine, number);
        continue;
      }
      if (isdigit(s))
      {
        number += s;
      }
      else
      {
        addNumberInLine(postfixLine, number);
        if (s == '(')
        {
          operators.push(s);
        }
        else if (isOperator(s))
        {
          while (!operators.empty() && getPriority(operators.back()) >= getPriority(s))
          {
            postfixLine += ' ';
            postfixLine += operators.back();
            operators.pop();
          }
          operators.push(s);
        }
        else if (s == ')')
        {
          while (!operators.empty() && operators.back() != '(')
          {
            postfixLine += ' ';
            postfixLine += operators.back();
            operators.pop();
          }
          operators.pop();
        }
      }
    }
    addNumberInLine(postfixLine, number);
    while (!operators.empty())
    {
      postfixLine += ' ';
      postfixLine += operators.back();
      operators.pop();
    }
    postfix.push(postfixLine);
  }
  return postfix;
}

kiselev::stackNumber kiselev::calculationExpr(queue& postfix)
{
  stackNumber results;
  while (!postfix.empty())
  {
    stackNumber numbers;
    std::string line = postfix.front();
    postfix.pop();
    std::string number;
    for (char s: line)
    {
      if (isspace(s))
      {
        addNumberInStack(number, numbers);
        continue;
      }
      if (isdigit(s))
      {
        number += s;
      }
      else if (isOperator(s))
      {
        addNumberInStack(number, numbers);
        if (numbers.size() < 2)
        {
          throw std::logic_error("Not enough operands");
        }
        long long int right = numbers.back();
        numbers.pop();
        long long int left = numbers.back();
        numbers.pop();
        long long int result = calculatingOperation(left, right, s);
        numbers.push(result);
      }
    }
    addNumberInStack(number, numbers);
    if (numbers.size() != 1)
    {
      throw std::logic_error("Too many operands");
    }
    results.push(numbers.back());
  }

  return results;
}

void kiselev::inputExprs(std::istream& input, queue& exprs)
{
  std::string line;
  while (std::getline(input, line))
  {
    if (line.empty())
    {
      continue;
    }
    exprs.push(line);
  }
}

std::ostream& kiselev::outputResults(std::ostream &output, stackNumber& results)
{
  if (!results.empty())
  {
    output << results.back();
    results.pop();
    while (!results.empty())
    {
      output << " " << results.back();
      results.pop();
    }
  }
  return output;
}
