#include "convertExpressions.hpp"
#include <cctype>

bool isOperator(char c)
{
  return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%');
}

int getPriority(char op)
{
  if (op == '+' || op == '-')
  {
    return 1;
  }
  else if (op == '*' || op == '/' || op == '%')
  {
    return 2;
  }
  else
  {
    return 0;
  }
}

lebedev::Queue< std::string > lebedev::convertInfToPost(const std::string& expr)
{
  Stack< char > ops;
  Queue< std::string > output;
  std::string temp;

  for (size_t i = 0; i < expr.size(); i++)
  {
    char c = expr[i];
    if (isspace(c))
    {
      continue;
    }

    if (isdigit(c))
    {
      temp += c;
      while (i + 1 < expr.size() && isdigit(expr[i+1]))
      {
        temp += expr[++i];
      }
      output.push(temp);
      temp.clear();
      continue;
    }

    if (c == '(')
    {
      ops.push(c);
      continue;
    }

    if (c == ')')
    {
      while (!ops.isEmpty() && ops.top() != '(')
      {
        output.push(std::string(1, ops.top()));
        ops.pop();
      }
      if (ops.isEmpty())
      {
        throw std::runtime_error("Mismatched parentheses!\n");
      }
      ops.pop();
      continue;
    }

    if (isOperator(c))
    {
      while (!ops.isEmpty() && ops.top() != '(' && getPriority(ops.top()) >= getPriority(c))
      {
        output.push(std::string(1, ops.top()));
        ops.pop();
      }
      ops.push(c);
      continue;
    }
    throw std::runtime_error("Invalid symbol!\n");
  }

  if (!temp.empty())
  {
    throw std::runtime_error("Invalid number!\n");
  }

  while (!ops.isEmpty())
  {
    if (ops.top() == '(')
    {
      throw std::runtime_error("Mismatched parentheses!\n");
    }
    output.push(std::string(1, ops.top()));
    ops.pop();
  }
  return output;
}

long long lebedev::calculatePost(Queue< std::string >& post)
{
  Stack< long long > ops;
  while (!post.isEmpty())
  {
    std::string token = post.top();
    post.pop();

    if (isdigit(token[0]))
    {
      ops.push(std::stoll(token));
    }
    else if (isOperator(token[0]))
    {
      if (ops.size() < 2)
      {
        throw std::runtime_error("Invalid expression!\n");
      }
      long long b = ops.top();
      ops.pop();
      long long a = ops.top();
      ops.pop();
      ops.push(lebedev::calculate(a, b, token[0]));
    }
    else
    {
      throw std::runtime_error("Invalid token!\n");
    }
  }
  if (ops.size() != 1)
  {
    throw std::runtime_error("Invalid expression!\n");
  }
  return ops.top();
}

lebedev::Stack< std::string > lebedev::inputExpressions(std::istream& input)
{
  Stack< std::string > res;
  std::string line;

  while (input)
  {
    std::getline(input, line);
    if (line.empty())
    {
      continue;
    }
    auto post = convertInfToPost(line);
    res.push(std::to_string(calculatePost(post)));
  }
  return res;
}
