#include <sstream>
#include <stdexcept>
#include <cctype>
#include "expression.hpp"
#include "stack.hpp"
namespace hismatova
{
  int precedence(char op)
  {
    if (op == '+' || op == '-')
    {
      return 1;
    }
    if (op == '*' || op == '/' || op == '%')
    {
      return 2;
    }
    return 0;
  }
  Queue< PostfixValue > infixToPostfix(const std::string& expr)
  {
    Stack< char > opStack;
    Queue< PostfixValue > outputQueue;
    std::istringstream iss(expr);
    std::string token;
    while (iss >> token)
    {
      if (isdigit(token[0]))
      {
        outputQueue.push(PostfixValue(std::stoi(token)));
      }
      else if (token == "+" || token == "-" || token == "*" || token == "/" || token == "%")
      {
        char currentOp = token[0];
        while (!opStack.empty() && precedence(opStack.top()) >= precedence(currentOp))
        {
          outputQueue.push(PostfixValue(opStack.pop()));
        }
        opStack.push(currentOp);
      }
      else if (token == "(")
      {
        opStack.push('(');
      }
      else if (token == ")")
      {
        while (!opStack.empty() && opStack.top() != '(')
        {
          outputQueue.push(PostfixValue(opStack.pop()));
        }
        if (!opStack.empty())
        {
          opStack.pop();
        }
      }
      else
      {
        throw std::runtime_error("Invalid token in expression");
      }
    }
    while (!opStack.empty())
    {
      outputQueue.push(PostfixValue(opStack.pop()));
    }
    return outputQueue;
  }
  int evaluatePostfix(Queue< PostfixValue >& postfix)
  {
    Stack< PostfixValue > evalStack;
    while (!postfix.empty())
    {
      PostfixValue token = postfix.pop();
      if (token.isNumeric())
      {
        evalStack.push(token);
      }
      else
      {
        if (evalStack.empty())
        {
          throw std::runtime_error("Malformed expression");
        }
        PostfixValue b = evalStack.pop();
        if (evalStack.empty())
        {
          throw std::runtime_error("Malformed expression");
        }
        PostfixValue a = evalStack.pop();
        switch (token.getOperator())
        {
          case '+': evalStack.push(a + b); break;
          case '-': evalStack.push(a - b); break;
          case '*': evalStack.push(a * b); break;
          case '/':
            if (b.getValue() == 0)
            {
              throw std::runtime_error("Division by zero");
            }
            evalStack.push(a / b);
            break;
          case '%':
            if (b.getValue() == 0)
            {
              throw std::runtime_error("Modulo by zero");
            }
            evalStack.push(a % b);
            break;
          default: throw std::runtime_error("Unknown operator");
        }
      }
    }
    if (evalStack.empty())
    {
      throw std::runtime_error("Malformed expression");
    }
    return evalStack.pop().getValue();
  }
}
