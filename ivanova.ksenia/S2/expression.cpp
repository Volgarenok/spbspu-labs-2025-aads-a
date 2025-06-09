#include <algorithm>
#include <exception>
#include <limits>
#include <stdexcept>

#include "expression.hpp"
#include "queue.hpp"
#include "stack.hpp"

inline bool isNumber(const std::string& token)
{
  if (token.empty() || (token.size() == 1 && !std::isdigit(token[0])))
  {
    return false;
  }
  size_t start = token[0] == '+' || token[0] == '-' ? 1 : 0;
  bool flag = true;
  for (size_t i = start; i < token.size() && flag; ++i)
  {
    flag = std::isdigit(token[i]);
  }
  return flag;
}

inline bool isOperation(const std::string& token)
{
  return token == "+" || token == "-" || token == "*" || token == "/" || token == "%";
}

inline int prior(const std::string& op)
{
  if (op == "+" || op == "-")
  {
    return 1;
  }
  if (op == "*" || op == "/" || op == "%")
  {
    return 2;
  }
  return 0;
}

inline ivanova::Queue<std::string> getTokens(const std::string& line)
{
  ivanova::Queue<std::string> tokens;
  size_t prev = 0;
  for (size_t i = 0; i <= line.size(); ++i)
  {
    if (i == line.size() || std::isspace(line[i]))
    {
      if (prev != i)
      {
        tokens.push(line.substr(prev, i - prev));
      }
      prev = i + 1;
    }
  }
  return tokens;
}

inline ivanova::Queue<std::string> getPostfix(ivanova::Queue<std::string>& tokens)
{
  ivanova::Queue<std::string> postfix;
  ivanova::Stack<std::string> operations;
  while (!tokens.empty())
  {
    std::string token = tokens.front();
    tokens.pop();
    if (isNumber(token))
    {
      postfix.push(token);
    }
    else if (isOperation(token))
    {
      while (!operations.empty() &&
              isOperation(operations.top()) &&
              prior(operations.top()) >= prior(token))
      {
        postfix.push(operations.top());
        operations.pop();
      }
      operations.push(token);
    }
    else if (token == "(")
    {
      operations.push(token);
    }
    else if (token == ")")
    {
      while (!operations.empty() && operations.top() != "(")
      {
        postfix.push(operations.top());
        operations.pop();
      }
      if (!operations.empty())
      {
        operations.pop();
      }
      else
      {
        std::string message = "not balanced brackets";
        throw std::logic_error(message);
      }
    }
    else
    {
      std::string message = "unrknown token '" + token + "'";
      throw std::logic_error(message);
    }
  }
  while (!operations.empty())
  {
    if (operations.top() == "(")
    {
      throw std::logic_error("not balanced brackets");
    }
    postfix.push(operations.top());
    operations.pop();
  }
  return postfix;
}

inline int64_t saveAdd(int64_t a, int64_t b)
{
  if ((b > 0 && a > std::numeric_limits<int64_t>::max() - b) ||
      (b < 0 && a < std::numeric_limits<int64_t>::min() - b))
  {
    throw std::logic_error("addition overflow");
  }
  return a + b;
}

inline int64_t saveSub(int64_t a, int64_t b)
{
  if ((b < 0 && a > std::numeric_limits<int64_t>::max() + b) ||
      (b > 0 && a < std::numeric_limits<int64_t>::min() + b))
  {
    throw std::logic_error("subtraction overflow");
  }
  return a - b;
}

inline int64_t saveMul(int64_t a, int64_t b)
{
  if (a > 0)
  {
    if (b > 0)
    {
      if (a > std::numeric_limits<int64_t>::max() / b)
      {
        throw std::logic_error("multiplication overflow");
      }
    }
    else if (b < 0)
    {
      if (b < std::numeric_limits<int64_t>::min() / a)
      {
        throw std::logic_error("multiplication overflow");
      }
    }
  }
  else if (a < 0)
  {
    if (b > 0)
    {
      if (a < std::numeric_limits<int64_t>::min() / b)
      {
        throw std::logic_error("multiplication overflow");
      }
    }
    else if (b < 0)
    {
      if (a < std::numeric_limits<int64_t>::max() / b)
      {
        throw std::logic_error("multiplication overflow");
      }
    }
  }
  return a * b;
}

inline int64_t saveDiv(int64_t a, int64_t b)
{
  if (b == 0)
  {
    throw std::logic_error("division by zero");
  }
  if (a == std::numeric_limits<int64_t>::min() && b == -1)
  {
    throw std::logic_error("division overflow");
  }
  return a / b;
}

inline int64_t saveMod(int64_t a, int64_t b)
{
  if (b == 0)
  {
    throw std::logic_error("modulo by zero");
  }
  if (a == std::numeric_limits<int64_t>::min() && b == -1)
  {
    throw std::logic_error("modulo overflow");
  }
  return a % b;
}

inline int64_t doOperation(int64_t a, int64_t b, const std::string& op)
{
  if (op == "+")
  {
    return saveAdd(a, b);
  }
  if (op == "-")
  {
    return saveSub(a, b);
  }
  if (op == "*")
  {
    return saveMul(a, b);
  }
  if (op == "/")
  {
    return saveDiv(a, b);
  }
  if (op == "%")
  {
    return saveMod(a, b);
  }
  throw std::logic_error("unknown operation '" + op + "'");
}

inline int64_t evalPostfix(ivanova::Queue<std::string>& postfix)
{
  ivanova::Stack<int64_t> stack;
  while (!postfix.empty())
  {
    std::string token = postfix.front();
    postfix.pop();
    if (isOperation(token))
    {
      if (stack.size() < 2)
      {
        std::string message = "binary operation '" + token + "' expected 2 arguments, but was one";
        throw std::logic_error(message);
      }
      int64_t b = stack.top(); stack.pop();
      int64_t a = stack.top(); stack.pop();
      int64_t c = doOperation(a, b, token);
      stack.push(c);
    }
    else
    {
      stack.push(std::stoll(token));
    }
  }
  if (stack.size() > 1)
  {
    throw std::logic_error("wrong expression");
  }
  return stack.top();
}

int64_t ivanova::countExpression(const std::string& expression)
{
  ivanova::Queue<std::string> tokens = getTokens(expression);
  ivanova::Queue<std::string> postfix = getPostfix(tokens);
  int64_t value = evalPostfix(postfix);
  return value;
}
