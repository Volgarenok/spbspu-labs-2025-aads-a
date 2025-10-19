#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include "expression-queue.hpp"
#include "calculation-stack.hpp"

namespace
{
  using namespace guseynov;
  void inputExpressions(std::istream & in, Queue< Queue< std::string > > & expressionsQueue)
  {
    std::string line;
    while (std::getline(in, line))
    {
      Queue< std::string > infixExpression;
      if (line.empty())
      {
        continue;
      }
      size_t start = 0;
      size_t end = line.find(' ');
      while (end != std::string::npos)
      {
        infixExpression.push(line.substr(start, end - start));
        start = end + 1;
        end = line.find(' ', start);
      }
      infixExpression.push(line.substr(start));
      if (!infixExpression.empty())
      {
        expressionsQueue.push(infixExpression);
      }
    }
  }

  bool isOperator(const std::string &op)
  {
    return op == "*" || op == "/" || op == "%" || op == "+" || op == "-";
  }

  int getOperatorPriority(const std::string &op)
  {
    if (op == "+" || op == "-")
    {
      return 1;
    }
    else if (op == "*" || op == "/" || op == "%")
    {
      return 2;
    }
    else
    {
      return 0;
    }
  }

  bool hasHigherOrEqualPriority(const std::string & op1, const std::string & op2)
  {
    return getOperatorPriority(op1) >= getOperatorPriority(op2);
  }

  Queue< Queue< std::string > > convertInfixToPostfix(Queue< Queue< std::string > > & expressionsQueue)
  {
    Queue< Queue< std::string > > postfixQueue;
    while (!expressionsQueue.empty())
    {
      Queue< std::string > infixExpression = expressionsQueue.front();
      expressionsQueue.pop();
      Queue< std::string > postfixExpression;
      Stack< std::string > operatorStack;
      while (!infixExpression.empty())
      {
        std::string token = infixExpression.front();
        infixExpression.pop();
        try
        {
          std::stoll(token);
          operatorStack.push(token);
        }
        catch (const std::exception &)
        {
          if (token == "(")
          {
            operatorStack.push(token);
          }
          else if (token == ")")
          {
            while (!operatorStack.empty() && operatorStack.top() != "(")
            {
              postfixExpression.push(operatorStack.top());
              operatorStack.pop();
            }
            if (operatorStack.empty())
            {
              throw std::logic_error("Invalid brackets placement");
            }
            operatorStack.pop();
          }
          else if (isOperator(token))
          {
            while (!operatorStack.empty() && operatorStack.top() != "(" &&
                   hasHigherOrEqualPriority(token, operatorStack.top()))
            {
              postfixExpression.push(operatorStack.top());
              operatorStack.pop();
            }
            operatorStack.push(token);
          }
          else
          {
            throw std::logic_error("Invalid expression token");
          }
        }
      }
      while (!operatorStack.empty())
      {
        if (operatorStack.top() == "(")
        {
          throw std::logic_error("Mismatched brackets");
        }
        postfixExpression.push(operatorStack.top());
        operatorStack.pop();
      }
      postfixQueue.push(postfixExpression);
    }
    return postfixQueue;
  }

  long long addWithOverflowCheck(long long a, long long b)
  {
    const long long max = std::numeric_limits< long long >::max();
    if (a > max - b)
    {
      throw std::overflow_error("Addition overflow");
    }
    return a + b;
  }

  long long subtractWithOverflowCheck(long long a, long long b)
  {
    const long long min = std::numeric_limits< long long >::min();
    if (a < min + b)
    {
      throw std::overflow_error("Subtraction overflow");
    }
    return a - b;
  }

  long long multiplyWithOverflowCheck(long long a, long long b)
  {
    const long long max = std::numeric_limits< long long >::max();
    const long long min = std::numeric_limits< long long >::min();
    if ((a > max / b) || (a < min / b))
    {
      throw std::overflow_error("Multiplication overflow");
    }
    return a * b;
  }

  long long divideWithOverflowCheck(long long a, long long b)
  {
    const long long min = std::numeric_limits< long long >::min();
    if (a == min && b == -1)
    {
      throw std::overflow_error("Division overflow");
    }
    return a / b;
  }

  long long moduloWithAdjustment(long long a, long long b)
  {
    long long result = a % b;
    if (result < 0)
    {
      result += std::abs(b);
    }
    return result;
  }

  long long calculateOperation(std::string op, long long a, long long b)
  {
    if (op == "+")
    {
      return addWithOverflowCheck(a, b);
    }
    else if (op == "-")
    {
      return subtractWithOverflowCheck(a, b);
    }
    else if (op == "*")
    {
      return multiplyWithOverflowCheck(a, b);
    }
    else if (op == "/")
    {
      return divideWithOverflowCheck(a, b);
    }
    else
    {
      return moduloWithAdjustment(a, b);
    }
  }

  Stack< long long > evaluateExpressions(Queue< Queue< std::string > > & postfixQueue)
  {
    Stack< long long > results;
    while (!postfixQueue.empty())
    {
      Queue< std::string > postfixExpression = postfixQueue.front();
      postfixQueue.pop();
      Stack< long long > operandStack;
      while (!postfixExpression.empty())
      {
        std::string token = postfixExpression.front();
        postfixExpression.pop();
        try
        {
          operandStack.push(std::stoll(token));
        }
        catch (const std::exception &)
        {
          long long b = operandStack.top();
          operandStack.pop();
          long long a = operandStack.top();
          operandStack.pop();
          long long result = calculateOperation(token, a, b);
          operandStack.push(result);
        }
      }
      results.push(operandStack.top());
    }
    return results;
  }

  void outputResults(std::ostream & out, Stack< long long > & results)
  {
    if (!results.empty())
    {
      out << results.top();
      results.pop();
      while (!results.empty())
      {
        out << " " << results.top();
        results.pop();
      }
    }
  }
}

int main(int argc, char * argv[])
{
  using namespace guseynov;
  Queue< Queue< std::string > > infixExpressions;
  Queue< Queue< std::string > > postfixExpressions;
  Stack< long long > calculationResults;
  try
  {
    if (argc > 1)
    {
      std::ifstream inputFile(argv[1]);
      inputExpressions(inputFile, infixExpressions);
    }
    else
    {
      inputExpressions(std::cin, infixExpressions);
    }
    postfixExpressions = convertInfixToPostfix(infixExpressions);
    calculationResults = evaluateExpressions(postfixExpressions);
  }
  catch (const std::exception & e)
  {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
  outputResults(std::cout, calculationResults);
  std::cout << "\n";
}
