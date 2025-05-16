#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <climits>
#include "stack.hpp"
#include "queue.hpp"

namespace
{
  bool isOperator(char c)
  {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
  }

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

  duhanina::Queue< std::string > infixToPostfix(const std::string& infix)
  {
    duhanina::Stack< char > operators;
    duhanina::Queue< std::string > output;
    std::stringstream ss(infix);
    std::string token;
    while (ss >> token)
    {
      if (std::isdigit(token[0]))
      {
        output.push(token);
      }
      else if (token[0] == '(')
      {
        operators.push(token[0]);
      }
      else if (token[0] == ')')
      {
        while (!operators.empty() && operators.top() != '(')
        {
          output.push(std::string(1, operators.top()));
          operators.pop();
        }
        if (!operators.empty() && operators.top() == '(')
        {
          operators.pop();
        }
        else
        {
          throw std::runtime_error("Mismatched parentheses");
        }
      }
      else if (isOperator(token[0]))
      {
        while (!operators.empty() && precedence(operators.top()) >= precedence(token[0]))
        {
          output.push(std::string(1, operators.top()));
          operators.pop();
        }
        operators.push(token[0]);
      }
      else
      {
        throw std::runtime_error("Invalid token");
      }
    }
    while (!operators.empty())
    {
      if (operators.top() == '(' || operators.top() == ')')
      {
        throw std::runtime_error("Mismatched parentheses");
      }
      output.push(std::string(1, operators.top()));
      operators.pop();
    }
    return output;
  }

  long long calcPostfix(duhanina::Queue< std::string >& postfix)
  {
    duhanina::Stack< long long > operands;
    while (!postfix.empty())
    {
      std::string token = postfix.front();
      postfix.pop();
      if (std::isdigit(token[0]))
      {
        operands.push(std::stoll(token));
      }
      else if (isOperator(token[0]))
      {
        long long operand2 = operands.top();
        operands.pop();
        long long operand1 = operands.top();
        operands.pop();
        switch (token[0])
        {
        case '+':
          if ((operand1 > 0 && operand2 > 0 && operand1 > LLONG_MAX - operand2) ||
              (operand1 < 0 && operand2 < 0 && operand1 < LLONG_MIN - operand2))
          {
            throw std::runtime_error("Overflow in addition");
          }
          operands.push(operand1 + operand2);
          break;
        case '-':
          if ((operand2 > 0 && operand1 < LLONG_MIN + operand2) ||
              (operand2 < 0 && operand1 > LLONG_MAX + operand2))
          {
            throw std::runtime_error("Overflow in subtraction");
          }
          operands.push(operand1 - operand2);
          break;
        case '*':
          if (operand1 > 0 && operand2 > 0 && operand1 > LLONG_MAX / operand2)
          {
            throw std::runtime_error("Overflow in multiplication");
          }
          if (operand1 < 0 && operand2 < 0 && operand1 < LLONG_MAX / operand2)
          {
            throw std::runtime_error("Overflow in multiplication");
          }
          if (operand1 > 0 && operand2 < 0 && operand2 < LLONG_MIN / operand1)
          {
            throw std::runtime_error("Overflow in multiplication");
          }
          if (operand1 < 0 && operand2 > 0 && operand1 < LLONG_MIN / operand2)
          {
            throw std::runtime_error("Overflow in multiplication");
          }
          operands.push(operand1 * operand2);
          break;
        case '/':
          if (operand2 == 0)
          {
            throw std::runtime_error("Division by zero");
          }
          operands.push(operand1 / operand2);
          break;
        case '%':
          if (operand2 == 0)
          {
            throw std::runtime_error("Modulus by zero");
          }
          if (operand1 % operand2 < 0)
          {
            operands.push(operand1 % operand2 + operand2);
            break;
          }
          operands.push(operand1 % operand2);
          break;
        default:
          throw std::runtime_error("Invalid operator");
        }
      }
      else
      {
        throw std::runtime_error("Invalid token");
      }
    }
    if (operands.size() != 1)
    {
      throw std::runtime_error("Invalid expression");
    }
    return operands.top();
  }

  void processExpressions(std::istream& input)
  {
    duhanina::Stack< long long > results;
    std::string line;
    while (std::getline(input, line))
    {
      if (line.empty())
      {
        continue;
      }
      duhanina::Queue< std::string > postfix = infixToPostfix(line);
      long long result = calcPostfix(postfix);
      results.push(result);
    }
    if (!results.empty())
    {
      std::cout << results.top();
      results.pop();
      while (!results.empty())
      {
        std::cout << " " << results.top();
        results.pop();
      }
    }
  }
}

int main(int argc, char* argv[])
{
  if (argc > 2)
  {
    std::cerr << "Too many arguments\n";
    return 1;
  }
  try
  {
    if (argc == 2)
    {
      std::ifstream file(argv[1]);
      if (!file)
      {
        std::cerr << "Could not open file\n";
        return 1;
      }
      processExpressions(file);
      std::cout << "\n";
    }
    else
    {
      processExpressions(std::cin);
      std::cout << "\n";
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
  return 0;
}
