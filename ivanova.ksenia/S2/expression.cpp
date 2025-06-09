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
