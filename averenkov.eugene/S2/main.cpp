#include <iostream>
#include <limits>
#include <fstream>
#include <string>
#include <cctype>
#include <stdexcept>
#include "stack.hpp"
#include "queue.hpp"

bool isOverflow(long long a, long long b)
{
  const long long int max = std::numeric_limits< long long >::max();
  const long long int min = std::numeric_limits< long long >::min();
  if (a == 0 || b == 0)
  {
    return false;
  }
  if (a > 0)
  {
    if (b > 0)
    {
      return a > max / b;
    }
    else
    {
      return b < min / a;
    }
  }
  else
  {
    if (b > 0)
    {
      return a < min / b;
    }
    else
    {
      return a < max / b;
    }
  }
}

bool isAdditionOverflow(long long a, long long b)
{
  if (b > 0)
  {
    return a > std::numeric_limits< long long >::max() - b;
  }
  else if (b < 0)
  {
    return a < std::numeric_limits< long long >::min() - b;
  }
  return false;
}

bool isSubtractionOverflow(long long a, long long b)
{
  if (b > 0)
  {
    return a < std::numeric_limits< long long >::min() + b;
  }
  else if (b < 0)
  {
    return a > std::numeric_limits< long long >::max() + b;
  }
  return false;
}

bool isOperator(char c)
{
  return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
}

bool precedenceFirst(char first, char second)
{
  if ((first == '+' || first == '-') && (second == '*' || second == '/' || second == '%'))
  {
    return false;
  }
  return true;
}

std::string readToken(const std::string& str, size_t& pos)
{
  pos = str.find_first_not_of(" \t\n\r", pos);
  if (pos == std::string::npos)
  {
    return "";
  }
  if (isOperator(str[pos]) || str[pos] == '(' || str[pos] == ')')
  {
    return std::string(1, str[pos++]);
  }
  size_t start = pos;
  size_t end = str.find_first_of(" \t\n\r()", pos);
  if (end == std::string::npos)
  {
    end = str.size();
  }
  size_t op_pos = str.find_first_of("+-*/%^", pos);
  if (op_pos != std::string::npos && op_pos < end)
  {
    end = op_pos;
  }
  std::string token = str.substr(start, end - start);
  pos = end;
  if (token == "-" && (start == 0 || str[start - 1] == '('))
  {
    std::string nextToken = readToken(str, pos);
    if (nextToken.empty())
    {
      throw std::runtime_error("Error");
    }
    return "-" + nextToken;
  }
  return token;
}

averenkov::Queue< std::string > infixToPostfix(const std::string& infix)
{
  averenkov::Stack< char > opStack;
  averenkov::Queue< std::string > outputQueue;
  size_t pos = 0;
  while (pos < infix.size())
  {
    std::string token = readToken(infix, pos);
    if (token.empty())
    {
      continue;
    }
    if (std::isdigit(token[0]) || (token[0] == '-' && token.size() > 1))
    {
      outputQueue.push(token);
    }
    else if (token == "(")
    {
      opStack.push('(');
    }
    else if (token == ")")
    {
      while (!opStack.empty() && opStack.top() != '(')
      {
        outputQueue.push(std::string(1, opStack.top()));
        opStack.pop();
      }
      if (opStack.empty())
      {
        throw std::runtime_error("Error");
      }
      opStack.pop();
    }
    else if (isOperator(token[0]))
    {
      while (!opStack.empty() && opStack.top() != '(' && precedenceFirst(opStack.top(), token[0]))
      {
        outputQueue.push(std::string(1, opStack.top()));
        opStack.pop();
      }
      opStack.push(token[0]);
    }
    else
    {
      throw std::runtime_error("Error");
    }
  }
  while (!opStack.empty())
  {
    if (opStack.top() == '(')
    {
      throw std::runtime_error("Error");
    }
    outputQueue.push(std::string(1, opStack.top()));
    opStack.pop();
  }
  return outputQueue;
}

long long evaluatePostfix(averenkov::Queue< std::string >& postfixQueue)
{
  averenkov::Stack< long long > evalStack;
  while (!postfixQueue.empty())
  {
    std::string token = postfixQueue.front();
    postfixQueue.pop();
    if (std::isdigit(token[0]) || (token[0] == '-' && token.size() > 1))
    {
      long long num = 0;
      bool negative = false;
      if (token[0] == '-')
      {
        negative = true;
      }
      num = std::stoll(token);
      evalStack.push(negative ? -num : num);
    }
    else if (isOperator(token[0]))
    {
      if (evalStack.size() < 2)
      {
        throw std::runtime_error("Error");
      }
      long long b = evalStack.top();
      evalStack.pop();
      long long a = evalStack.top();
      evalStack.pop();
      long long result;
      switch (token[0])
      {
        case '+':
          if (isAdditionOverflow(a, b))
          {
            throw std::runtime_error("Overflow error");
          }
          result = a + b;
          break;
        case '-':
          if (isSubtractionOverflow(a, b))
          {
            throw std::runtime_error("Overflow error");
          }
          result = a - b;
          break;
        case '*':
          if (isOverflow(a, b))
          {
            throw std::runtime_error("Overflow error");
          }
          result = a * b;
          break;
        case '/':
          if (b == 0)
          {
            throw std::runtime_error("Division by zero");
          }
          result = a / b;
          break;
        case '%':
          if (b == 0)
          {
            throw std::runtime_error("Error");
          }
          result = (a % b + b) % b;
          break;
        default:
          throw std::runtime_error("Error");
      }
      evalStack.push(result);
    }
    else
    {
      throw std::runtime_error("Invalid token");
    }
  }
  if (evalStack.size() != 1)
  {
    throw std::runtime_error("Expression error");
  }
  auto temp = evalStack.top();
  evalStack.pop();
  return temp;
}

void processExpressions(std::istream& input)
{
  averenkov::Stack< long long > results;
  std::string line;
  while (std::getline(input, line))
  {
    if (line.empty())
    {
      continue;
    }
    averenkov::Queue< std::string > postfixQueue = infixToPostfix(line);
    long long result = evaluatePostfix(postfixQueue);
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
  std::cout << "\n";

}

int main(int argc, char* argv[])
{
  try
  {
    if (argc > 2)
    {
      std::cerr << "Error\n";
      return 1;
    }

    if (argc == 2)
    {
      std::ifstream file(argv[1]);
      if (!file)
      {
        std::cerr << "Error\n";
        return 1;
      }
      processExpressions(file);
    }
    else
    {
      processExpressions(std::cin);
    }
  }
  catch (...)
  {
    std::cerr << "Error\n";
    return 1;
  }
  return 0;
}
