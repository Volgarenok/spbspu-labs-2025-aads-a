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

  bool higherEqualPrecedence(char op1, char op2)
  {
    if ((op1 == '+' || op1 == '-') && (op2 == '*' || op2 == '/' || op2 == '%'))
    {
      return false;
    }
    return true;
  }

duhanina::Queue<std::string> infixToPostfix(const std::string& infix)
{
  duhanina::Stack<char> operators;
  duhanina::Queue<std::string> output;
  size_t pos = 0;
  const size_t len = infix.length();

  while (pos < len)
  {
    // Пропускаем пробелы
    while (pos < len && infix[pos] == ' ')
    {
      pos++;
    }
    if (pos >= len)
    {
      break;
    }

    std::string token;
    if (std::isdigit(infix[pos]))
    {
      size_t start = pos;
      while (pos < len && std::isdigit(infix[pos]))
      {
        pos++;
      }
      token = infix.substr(start, pos - start);
      output.push(token);
    }
    else
    {
      char c = infix[pos++];
      token = std::string(1, c);

      if (c == '(')
      {
        operators.push(c);
      }
      else if (c == ')')
      {
        while (!operators.empty() && operators.top() != '(')
        {
          output.push(std::string(1, operators.top()));
          operators.pop();
        }
        if (operators.empty())
        {
          throw std::runtime_error("Mismatched parentheses");
        }
        operators.pop();
      }
      else if (isOperator(c))
      {
        while (!operators.empty() && operators.top() != '(' &&
               higherEqualPrecedence(operators.top(), c))
        {
          output.push(std::string(1, operators.top()));
          operators.pop();
        }
        operators.push(c);
      }
      else
      {
        throw std::runtime_error("Invalid token: " + token);
      }
    }
  }

  while (!operators.empty())
  {
    if (operators.top() == '(')
    {
      throw std::runtime_error("Mismatched parentheses");
    }
    output.push(std::string(1, operators.top()));
    operators.pop();
  }

  return output;
}

  long long checkedAdd(long long a, long long b)
  {
    if ((b > 0 && a > LLONG_MAX - b) || (b < 0 && a < LLONG_MIN - b))
    {
      throw std::runtime_error("Overflow");
    }
    return a + b;
  }

  long long checkedSub(long long a, long long b)
  {
    if ((b > 0 && a < LLONG_MIN + b) || (b < 0 && a > LLONG_MAX + b))
    {
      throw std::runtime_error("Overflow");
    }
    return a - b;
  }

  long long checkedMul(long long a, long long b)
  {
    if (a > 0)
    {
      if (b > 0 && a > LLONG_MAX / b)
      {
        throw std::runtime_error("Overflow");
      }
      if (b < 0 && b < LLONG_MIN / a)
      {
        throw std::runtime_error("Overflow");
      }
    }
    else
    {
      if (b > 0 && a < LLONG_MIN / b)
      {
        throw std::runtime_error("Overflow");
      }
      if (b < 0 && a < LLONG_MAX / b)
      {
        throw std::runtime_error("Overflow");
      }
    }
    return a * b;
  }

  long long checkedDiv(long long a, long long b)
  {
    if (b == 0)
    {
      throw std::runtime_error("Division by zero");
    }
    return a / b;
  }

 long long checkedMod(long long a, long long b)
 {
    if (b == 0)
    {
      throw std::runtime_error("Modulus by zero");
    }
    long long result = a % b;
    if (result < 0)
    {
      return result + b;
    }
    return result;
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
          operands.push(checkedAdd(operand1, operand2));
          break;
        case '-':
          operands.push(checkedSub(operand1, operand2));
          break;
        case '*':
          operands.push(checkedMul(operand1, operand2));
          break;
        case '/':
          operands.push(checkedDiv(operand1, operand2));
          break;
        case '%':
          operands.push(checkedMod(operand1, operand2));
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

