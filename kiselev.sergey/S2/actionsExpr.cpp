#include "actionsExpr.hpp"
#include <limits>
#include <stdexcept>

namespace
{
  int getPriority(const std::string& operation)
  {
    if (operation == "+" || operation == "-")
    {
      return 1;
    }
    else if (operation == "*" || operation == "/" || operation == "%")
    {
      return 2;
    }
    return 0;
  }

  bool isOperator(const std::string& c)
  {
    return c == "+" || c == "-" || c == "/" || c == "*" || c == "%";
  }

  void checkMultiplicate(long long int a, long long int b)
  {
    long long int max = std::numeric_limits< long long int >::max();
    long long int min = std::numeric_limits< long long int >::min();
    bool isOverflow = false;
    if (a > 0 && b > 0 && a > max / b)
    {
      isOverflow = true;
    }
    else if (a > 0 && b < 0 && b < min / a)
    {
      isOverflow = true;
    }
    else if (a < 0 && b > 0 && a < min / b)
    {
      isOverflow = true;
    }
    else if (a < 0 && b < 0 && b < max / a)
    {
      isOverflow = true;
    }
    if (isOverflow)
    {
      throw std::logic_error("Overflow");
    }
  }
  void checkSum(long long int a, long long int b)
  {
    long long int max = std::numeric_limits< long long int >::max();
    if (a > max - b)
    {
      throw std::logic_error("Overflow");
    }
  }
  void checkDifference(long long int a, long long b)
  {
    long long int min = std::numeric_limits< long long int >::min();
    if (a < min + b)
    {
      throw std::logic_error("Underflow");
    }
  }
  void checkDivision(long long int b)
  {
    if (b == 0)
    {
      throw std::logic_error("Dividing by 0");
    }
  }
  long long int calculatingOperation(long long int a, long long b, const std::string& operation)
  {
    switch (operation.front())
    {
    case '+':
      checkSum(a, b);
      return a + b;
    case '-':
      checkDifference(a, b);
      return a - b;
    case '*':
      checkMultiplicate(a, b);
      return a * b;
    case '/':
      checkDivision(b);
      return a / b;
    case '%':
      checkDivision(b);
      return a >= 0 ? a % b : (b - std::abs(a % b)) % b;
    default:
      throw std::logic_error("Invalid operation");
    }
  }

}

kiselev::queue kiselev::convertExpr(queue& infix)
{
  queue postfixExprs;
  while (!infix.empty())
  {
    Expr infixExpr = infix.front();
    infix.pop();
    Expr postfixExpr;
    Stack< std::string > operators;
    while (!infixExpr.empty())
    {
      std::string partInfix = infixExpr.front();
      infixExpr.pop();
      if (partInfix == "(")
      {
        operators.push(partInfix);
      }
      else if (partInfix == ")")
      {
        while (!operators.empty() && operators.top() != "(")
        {
          postfixExpr.push(operators.top());
          operators.pop();
        }
        operators.pop();
      }
      else if (isOperator(partInfix))
      {
        while (!operators.empty() && getPriority(operators.top()) >= getPriority(partInfix))
        {
          postfixExpr.push(operators.top());
          operators.pop();
        }
        operators.push(partInfix);
      }
      else
      {
        postfixExpr.push(partInfix);
      }
    }
    while (!operators.empty())
    {
      postfixExpr.push(operators.top());
      operators.pop();
    }
    postfixExprs.push(postfixExpr);
  }
  return postfixExprs;
}

kiselev::stackNumber kiselev::calculationExpr(queue& postfixExprs)
{
  stackNumber results;
  while (!postfixExprs.empty())
  {
    stackNumber numbers;
    Expr postfixExpr = postfixExprs.front();
    postfixExprs.pop();
    while (!postfixExpr.empty())
    {
      std::string partPostfix = postfixExpr.front();
      postfixExpr.pop();
      if (isOperator(partPostfix))
      {
        if (numbers.size() < 2)
        {
          throw std::logic_error("Not enough operands");
        }
        long long int right = numbers.top();
        numbers.pop();
        long long int left = numbers.top();
        numbers.pop();
        long long int result = calculatingOperation(left, right, partPostfix);
        numbers.push(result);
      }
      else
      {
        try
        {
          long long int number = stoll(partPostfix);
          numbers.push(number);
        }
        catch (...)
        {
          throw std::logic_error("Invalid number");
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

void kiselev::inputExprs(std::istream& input, queue& exprs)
{
  std::string line;
  while (std::getline(input, line))
  {
    if (line.empty())
    {
      continue;
    }
    Expr infixExpr;
    std::string partExpr;
    size_t start = 0;
    size_t end = line.find(' ');
    while (end != std::string::npos)
    {
      partExpr = line.substr(start, end - start);
      if (!partExpr.empty())
      {
        infixExpr.push(partExpr);
      }
      start = end + 1;
      end = line.find(' ', start);
    }
    partExpr = line.substr(start);
    if (!partExpr.empty())
    {
      infixExpr.push(partExpr);
    }
    exprs.push(infixExpr);
  }
}

std::ostream& kiselev::outputResults(std::ostream& output, stackNumber& results)
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
