#include "processMathExpressions.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

namespace
{
  bool isNumber(const std::string& str)
  {
    try
    {
      std::stoll(str);
    }
    catch (const std::exception& e)
    {
      return false;
    }
    for (size_t i = 0; i < str.size(); ++i)
    {
      if (!std::isdigit(str[i]))
      {
        return false;
      }
    }
    return true;
  }

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

  bool isSameSign(long long int a, long long int b)
  {
    return sign(a) * sign(b) > 0;
  }

  long long int sum(long long int lhs, long long int rhs)
  {
    if (isSameSign(lhs, rhs) && lhs > 0 && max - lhs >= rhs)
    {
      return lhs + rhs;
    }
    else if (isSameSign(lhs, rhs) && lhs < 0 && min - lhs <= rhs)
    {
      return lhs + rhs;
    }
    else if (lhs == 0 || rhs == 0)
    {
      return lhs + rhs;
    }
    else if (!isSameSign(lhs, rhs))
    {
      return lhs + rhs;
    }
    throw std::invalid_argument("Overflow!");
  }

  long long int diff(long long int lhs, long long int rhs)
  {
    if (isSameSign(lhs, rhs) && lhs > 0 && min + rhs <= lhs)
    {
      return lhs - rhs;
    }
    else if (isSameSign(lhs, rhs) && lhs < 0 && max - lhs >= rhs)
    {
      return lhs - rhs;
    }
    else if (lhs == 0 || rhs == 0)
    {
      return lhs - rhs;
    }
    else if (!isSameSign(lhs, rhs))
    {
      return lhs - rhs;
    }
    throw std::invalid_argument("Overflow!");
  }

  long long int mult(long long int lhs, long long int rhs)
  {
    if (isSameSign(lhs, rhs) && lhs > 0 && max / lhs >= rhs)
    {
      return lhs * rhs;
    }
    else if (isSameSign(lhs, rhs) && lhs < 0 && max / lhs >= std::abs(rhs))
    {
      return lhs * rhs;
    }
    else if (lhs == 0 || rhs == 0)
    {
      return 0;
    }
    else if (!isSameSign(lhs, rhs) && std::abs(min / lhs) >= std::abs(rhs))
    {
      return lhs * rhs;
    }
    throw std::logic_error("Overflow!");
  }

  long long int div(long long int lhs, long long int rhs)
  {
    if (rhs == 0)
    {
      throw std::invalid_argument("Division by zero");
    }
    else if (isSameSign(lhs, rhs) && lhs < 0 && lhs != min && rhs != -1)
    {
      return lhs / rhs;
    }
    else
    {
      return lhs / rhs;
    }
    throw std::invalid_argument("Overflow!");
  }

  long long int mod(long long int lhs, long long int rhs)
  {
    if (rhs == 0)
    {
      throw std::invalid_argument("Division by zero");
    }
    else if (lhs < 0)
    {
      return rhs - std::abs(lhs) % rhs;
    }
    else
    {
      return lhs % rhs;
    }
    throw std::invalid_argument("Overflow!");
  }

  long long int doOperation(long long int lhs, long long int rhs, const std::string& op)
  {
    if (op == "+")
    {
      return sum(lhs, rhs);
    }
    else if (op == "-")
    {
      return diff(lhs, rhs);
    }
    else if (op == "*")
    {
      return mult(lhs, rhs);
    }
    else if (op == "/")
    {
      return div(lhs, rhs);
    }
    else if (op == "%")
    {
      return mod(lhs, rhs);
    }
    else
    {
      throw std::invalid_argument("Incorrect expression!");
    }
  }

  bool isHighPriorityOp(const std::string& str)
  {
    return str == "*" || str == "/" || str == "%";
  }

  bool isLowPriorityOp(const std::string& str)
  {
    return str == "+" || str == "-";
  }
}

long long int krylov::calculatePostfix(Queue< std::string >* postfixQueue)
{
  Stack< long long int > calculatingStack;
  std::string symb;
  while (!postfixQueue->empty())
  {
    symb = postfixQueue->front();
    postfixQueue->pop();
    if (isNumber(symb))
    {
      calculatingStack.push(std::stoll(symb));
    }
    else
    {
      long long int rightOperand = calculatingStack.top();
      calculatingStack.pop();
      long long int leftOperand = calculatingStack.top();
      calculatingStack.pop();
      calculatingStack.push(doOperation(leftOperand, rightOperand, symb));
    }
  }
  if (calculatingStack.size() != 1)
  {
    throw std::logic_error("Incorrect expression!");
  }
  return calculatingStack.top();
}

std::istream& krylov::calculateExpression(std::istream& in)
{
  std::string str;
  Stack< long long int > stackOfValues;
  while (in)
  {
    std::getline(in, str);
    if (str.empty())
    {
      continue;
    }
    stackOfValues.push(calculateInfix(str));
  }
  if (stackOfValues.empty())
  {
    std::cout << "\n";
    return in;
  }
  std::cout << stackOfValues.top();
  stackOfValues.pop();
  while (!stackOfValues.empty())
  {
    std::cout << " " << stackOfValues.top();
    stackOfValues.pop();
  }
  std::cout << "\n";
  return in;
}

long long int krylov::calculateInfix(const std::string& str)
{
  Queue< std::string >* postfixExpression = getPostfixExpression(str);
  long long int res = 0;
  try
  {
    res = calculatePostfix(postfixExpression);
  }
  catch (const std::exception& e)
  {
    delete postfixExpression;
    throw;
  }
  delete postfixExpression;
  return res;
}

krylov::Queue< std::string >* krylov::getPostfixExpression(const std::string& str)
{
  Queue< std::string > infixQueue;
  std::stringstream ss(str);
  std::string symb;
  while (ss >> symb)
  {
    infixQueue.push(symb);
  }
  Stack< std::string > processingStack;
  Queue< std::string >* postfixQueue = new Queue< std::string >;
  bool bracketActive = false;
  while (!infixQueue.empty())
  {
    std::string symb = infixQueue.front();
    infixQueue.pop();
    if (symb == "(")
    {
      processingStack.push(symb);
      bracketActive = true;
    }
    else if (symb == ")")
    {
      if (bracketActive == false)
      {
        delete postfixQueue;
        throw std::logic_error("Incorrect expression!");
      }
      while (processingStack.top() != "(")
      {
        postfixQueue->push(processingStack.top());
        processingStack.pop();
      }
      processingStack.pop();
      bracketActive = false;
    }
    else if (isNumber(symb))
    {
      postfixQueue->push(symb);
    }
    else if (isLowPriorityOp(symb))
    {
      if (!processingStack.empty())
      {
        if (processingStack.top() != "(")
        {
          postfixQueue->push(processingStack.top());
          processingStack.pop();
        }
      }
      processingStack.push(symb);
    }
    else if (isHighPriorityOp(symb))
    {
      if (!processingStack.empty())
      {
        if (isHighPriorityOp(processingStack.top()))
        {
          postfixQueue->push(processingStack.top());
          processingStack.pop();
        }
      }
      processingStack.push(symb);
    }
    else
    {
      delete postfixQueue;
      throw std::logic_error("Incorrect expression!");
    }
  }
  while (!processingStack.empty())
  {
    postfixQueue->push(processingStack.top());
    processingStack.pop();
  }
  if (bracketActive == true)
  {
    delete postfixQueue;
    throw std::logic_error("Incorrect expression!");
  }
  return postfixQueue;
}
