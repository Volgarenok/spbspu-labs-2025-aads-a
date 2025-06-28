#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cctype>
#include <climits>
#include <stack.hpp>
#include <queue.hpp>

namespace
{
  bool isOperator(const std::string& c)
  {
    return c == "+" || c == "-" || c == "*" || c == "/" || c == "%";
  }

  bool higherEqualPrecedence(const std::string& op1, const std::string& op2)
  {
    if ((op1 == "+" || op1 == "-") && (op2 == "*" || op2 == "/" || op2 == "%"))
    {
      return false;
    }
    return true;
  }

  void processToken(const std::string& token, duhanina::Stack< std::string >& operators, duhanina::Queue< std::string >& output)
  {
    if (token.empty())
    {
      return;
    }
    if (std::isdigit(token[0]))
    {
      output.push(token);
    }
    else if (token == "(")
    {
      operators.push(token);
    }
    else if (token == ")")
    {
      while (!operators.empty() && operators.top() != "(")
      {
        output.push(std::string(operators.top()));
        operators.pop();
      }
      if (operators.empty())
      {
        throw std::runtime_error("Mismatched parentheses");
      }
      operators.pop();
    }
    else if (isOperator(token))
    {
      while (!operators.empty() && operators.top() != "(" && higherEqualPrecedence(operators.top(), token))
      {
        output.push(std::string(operators.top()));
        operators.pop();
      }
      operators.push(token);
    }
    else
    {
      throw std::runtime_error("Invalid token");
    }
  }

  duhanina::Queue< std::string > infixToPostfix(const std::string& infix)
  {
    duhanina::Stack< std::string > operators;
    duhanina::Queue< std::string > output;
    size_t start = 0;
    size_t end = infix.find(' ');
    while (end != std::string::npos)
    {
      if (end != start)
      {
        processToken(infix.substr(start, end - start), operators, output);
      }
      start = end + 1;
      end = infix.find(' ', start);
    }
    if (start < infix.length())
    {
      processToken(infix.substr(start), operators, output);
    }
    while (!operators.empty())
    {
      if (operators.top() == "(")
      {
        throw std::runtime_error("Mismatched parentheses");
      }
      output.push(std::string(operators.top()));
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
      if (isOperator(token))
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
        operands.push(std::stoll(token));
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

