#include <iostream>
#include <climits>
#include <fstream>
#include <string>
#include <cctype>
#include <stdexcept>
#include "stack.hpp"
#include "queue.hpp"

bool isOverflow(long long a, long long b)
{
  if (a == 0 || b == 0)
  {
    return false;
  }
  if (a > 0)
  {
    if (b > 0)
    {
      return a > LLONG_MAX / b;
    }
    else
    {
      return b < LLONG_MIN / a;
    }
  }
  else
  {
    if (b > 0)
    {
      return a < LLONG_MIN / b;
    }
    else
    {
      return a < LLONG_MAX / b;
    }
  }
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
  while (pos < str.size() && std::isspace(str[pos]))
  {
    pos++;
  }
  if (pos >= str.size())
  {
    return "";
  }
  if (isOperator(str[pos]) || str[pos] == '(' || str[pos] == ')')
  {
    return std::string(1, str[pos++]);
  }
  std::string num;
  bool negative = false;
  if (str[pos] == '-' && (pos == 0 || str[pos-1] == '('))
  {
    negative = true;
    num += '-';
    pos++;
  }
  while (pos < str.size() && std::isdigit(str[pos]))
  {
    num += str[pos++];
  }
  if (num.empty() || (negative && num.size() == 1))
  {
    throw std::runtime_error("Error");
  }
  return num;
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
      size_t i = 0;
      if (token[0] == '-')
      {
        negative = true;
        i = 1;
      }
      for (; i < token.size(); ++i)
      {
        num = num * 10 + (token[i] - '0');
      }
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
          if (a > 0 && b > 0 && a > LLONG_MAX - b)
          {
            throw std::runtime_error("Overflow error");
          }
          result = a + b;
          break;
        case '-':
          if (b > 0 && a < LLONG_MIN + b)
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
    try
    {
      averenkov::Queue< std::string > postfixQueue = infixToPostfix(line);
      long long result = evaluatePostfix(postfixQueue);
      results.push(result);
    }
    catch (const std::exception& e)
    {
      throw std::runtime_error("Error in expression");
    }
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
