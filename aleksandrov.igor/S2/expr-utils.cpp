#include "expr-utils.hpp"
#include <stdexcept>
#include <iostream>
#include <sstream>
#include "io-utils.hpp"

namespace aleksandrov
{
  namespace detail
  {
    bool isLessImportant(const InfixPart& a, const InfixPart& b)
    {
      char op1 = a.getOperation();
      char op2 = b.getOperation();
      if (op1 == '+' || op1 == '-')
      {
        return op2 == '*' || op2 == '/' || op2 == '%';
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
    if ((!a && !b) || a == -b)
    {
      return 0ll;
    }

    long long int result = 0ll;
    if (sameSign(a, b) && a > 0)
    {
      if (LLIMax - a >= b)
      {
        result = a + b;
      }
    }
    else if (sameSign(a, b) && a < 0)
    {
      if (LLIMin - a <= b)
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
    if (!a || !b)
    {
      return 0ll;
    }

    if (sameSign(a, b))
    {
      if (a > 0ll)
      {
        if (a > LLIMax / b)
        {
          throw std::overflow_error("There was an overflow error!");
        }
      }
      else
      {
        if (a < LLIMin / b)
        {
          throw std::overflow_error("There was an overflow error!");
        }
      }
    }
    else
    {
      if (a > 0ll)
      {
        if (b < LLIMin / a)
        {
          throw std::overflow_error("There was an overflow error!");
        }
      }
      else
      {
        if (a < LLIMin / b)
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
    if (a < b && -a < b)
    {
      return 0ll;
    }

    long long int result = 0ll;
    if (a != LLIMin || b != -1)
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

  Queue< InfixPart > infixToPostfix(Queue< InfixPart >& infixExpr)
  {
    Queue< InfixPart > postfixExpr;
    Stack< InfixPart > stack;
    while (!infixExpr.empty())
    {
      InfixPart token = infixExpr.drop();
      if (token.isOperand())
      {
        postfixExpr.push(token);
      }
      else if (token.isOpeningBracket())
      {
        stack.push(token);
      }
      else if (token.isClosingBracket())
      {
        while (!stack.empty() && !stack.top().isOpeningBracket())
        {
          postfixExpr.push(stack.drop());
        }
        if (stack.empty())
        {
          throw std::logic_error("Mismatched brackets!");
        }
        stack.drop();
      }
      else if (token.isOperation())
      {
        while (!stack.empty() && stack.top().isOperation() && !isLessImportant(stack.top(), \
token))
        {
          postfixExpr.push(stack.drop());
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
      if (stack.top().isOpeningBracket() || stack.top().isClosingBracket())
      {
        throw std::logic_error("Mismatched brackets!");
      }
      postfixExpr.push(stack.drop());
    }
    return postfixExpr;
  }

  Queue< Queue< InfixPart > > infixesToPostfixes(Queue< Queue< InfixPart > >& infixExprs)
  {
    Queue< Queue< InfixPart > > postfixExprs;
    while (!infixExprs.empty())
    {
      Queue< InfixPart > infixExpr = infixExprs.front();
      Queue< InfixPart > postfixExpr = infixToPostfix(infixExpr);
      infixExprs.drop();
      postfixExprs.push(postfixExpr);
    }
    return postfixExprs;
  }

  long long int evalPostfixExpr(Queue< InfixPart >& postfixExpr)
  {
    Stack< long long int > stack;
    if (postfixExpr.size() == 1)
    {
      InfixPart token = postfixExpr.drop();
      if (token.isOperand())
      {
        return token.getOperand();
      }
    }
    while (!postfixExpr.empty())
    {
      InfixPart token = postfixExpr.drop();
      if (token.isOperand())
      {
        stack.push(token.getOperand());
      }
      else if (token.isOperation())
      {
        if (stack.size() < 2)
        {
          throw std::logic_error("Incorrect expression!");
        }
        long long int second = stack.drop();
        long long int first = stack.drop();
        long long int result = 0ll;
        char op = token.getOperation();
        if (op == '+')
        {
          result = sum(first, second);
        }
        else if (op == '-')
        {
          result = sum(first, -second);
        }
        else if (op == '*')
        {
          result = mult(first, second);
        }
        else if (op == '/')
        {
          result = div(first, second);
        }
        else if (op == '%')
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

  Stack< long long int > evalPostfixExprs(Queue< Queue< InfixPart > >& postfixExprs)
  {
    Stack < long long int > results;
    while (!postfixExprs.empty())
    {
      Queue< InfixPart > postfixExpr = postfixExprs.drop();
      long long int result = 0ll;
      result = evalPostfixExpr(postfixExpr);
      results.push(result);
    }
    return results;
  }

  Queue< InfixPart > getInfixExpr(std::istream& input)
  {
    Queue< InfixPart > infixExpr;
    InfixPart token('+');
    while (input >> token)
    {
      infixExpr.push(token);
    }
    return infixExpr;
  }

  Queue< Queue< InfixPart > > getInfixExprs(std::istream& input)
  {
    Queue< Queue< InfixPart > > infixExprs;
    std::string line;
    while (std::getline(input, line))
    {
      if (!line.empty())
      {
        std::istringstream iss(line);
        Queue< InfixPart > infixExpr = getInfixExpr(iss);
        infixExprs.push(infixExpr);
      }
    }
    return infixExprs;
  }
}

