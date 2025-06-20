#include <iostream>
#include <climits>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <stdexcept>
#include "stack.hpp"
#include "queue.hpp"


using namespace averenkov;

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


std::string infixToPostfix(const std::string& infix)
{
  Stack< char > stack;
  std::string postfix;
  std::string number;
  bool isNegative = false;

  for (size_t i = 0; i < infix.size(); ++i)
  {
    char ch = infix[i];

    if (std::isspace(ch)) continue;

    if (std::isdigit(ch))
    {
      number += ch;
    }
    else if (ch == '-' && (i == 0 || infix[i - 1] == '('))
    {
      isNegative = true;
    }
    else
    {
      if (!number.empty())
      {
        if (isNegative)
        {
          postfix += '-';
          isNegative = false;
        }
        postfix += number + " ";
        number.clear();
      }

      if (ch == '(')
      {
        stack.push(ch);
      }
      else if (ch == ')')
      {
        while (!stack.empty() && stack.top() != '(')
        {
          postfix += stack.drop();
          postfix += ' ';
        }
        if (stack.empty())
        {
          throw std::runtime_error("parentheses error");
        }
        stack.drop();
      }
      else if (isOperator(ch))
      {
        while (!stack.empty() && precedenceFirst(stack.top(), ch))
        {
          postfix += stack.drop();
          postfix += ' ';
        }
        stack.push(ch);
      }
      else
      {
        throw std::runtime_error("error character in expression");
      }
    }
  }

  if (!number.empty())
  {
    if (isNegative)
    {
      postfix += '-';
    }
    postfix += number + " ";
  }

  while (!stack.empty())
  {
    if (stack.top() == '(')
    {
      throw std::runtime_error("parentheses error");
    }
    postfix += stack.drop();
    postfix += ' ';
  }

  if (!postfix.empty() && postfix.back() == ' ')
  {
    postfix.pop_back();
  }

  return postfix;
}

long long evaluatePostfix(const std::string& postfix)
{
  Stack< long long > stack;
  std::istringstream iss(postfix);
  std::string token;

  while (iss >> token)
  {
    if (std::isdigit(token[0]))
    {
      stack.push(std::stoll(token));
    }
    else if (isOperator(token[0]))
    {
      try
      {
        long long b = stack.drop();
        long long a = stack.drop();
        long long result;

        switch (token[0])
        {
          case '+':
            if (a > 0 && b > 0 && a > LLONG_MAX - b)
            {
              throw std::runtime_error("overflow error");
            }
            result = a + b;
            break;
          case '-':
            if (b > 0 && a < LLONG_MIN + b)
            {
              throw std::runtime_error("overflow error");
            }
            result = a - b;
            break;
          case '*':
            if (isOverflow(a, b))
            {
              throw std::runtime_error("overflow error");
            }
            result = a * b;
            break;
          case '/':
            if (b == 0)
            {
              throw std::runtime_error("division zero");
            }
            result = a / b;
            break;
          case '%':
            if (b == 0)
            {
              throw std::runtime_error("modul zero");
            }
            result = (a % b + b) % b;
            break;
          default:
            throw std::runtime_error("invalid operator");
        }

        stack.push(result);
      }
      catch (const std::underflow_error& e)
      {
        throw std::runtime_error("expression error");
      }
    }
    else
    {
      throw std::runtime_error("invalid token");
    }
  }

  return stack.drop();
}


void processExpressions(std::istream& input)
{
  Stack< long long > results;
  std::string line;
  std::string token;

  while (input >> token)
  {
    if (token == "(" || token == ")" || isOperator(token[0]) || std::isdigit(token[0]))
    {
      line += token + " ";
    }
    else
    {
      throw std::runtime_error("invalid token");
    }

    if (input.peek() == '\n' || input.peek() == EOF)
    {
      if (!line.empty())
      {
        std::string postfix = infixToPostfix(line);
        long long result = evaluatePostfix(postfix);
        results.push(result);
        line.clear();
      }
    }
  }

  std::cout << results.drop();

  while (!results.empty())
  {
    std::cout << " " << results.drop();
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

  catch (const std::underflow_error& e)
  {
    std::cout << "\n";
    return 0;
  }

  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
  return 0;
}
