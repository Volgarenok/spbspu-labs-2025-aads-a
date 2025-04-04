#include "io_processing.hpp"
namespace
{
  bool isLoverPrecedence(const std::string & a, const std::string & b);
  bool isGreaterOrEqualPrecedence(const std::string & a, const std::string & b);
}
nikonov::Queue< std::string > nikonov::readInfixExpressions(std::istream & in)
{
  Queue< std::string > que;
  std::string infix;
  while (std::getline(in, infix))
  {
    if (!infix.empty())
    {
      que.push(infix);
    }
  }
  return que;
}
void nikonov::printResults(Stack< std::string > toOut, std::ostream & out)
{
  if (toOut.empty())
  {
    return;
  }
  out << toOut.top();
  toOut.pop();
  while (!toOut.empty())
  {
    out << ' ' << toOut.top();
    toOut.pop();
  }
  out << '\n';
}
std::string nikonov::convertToPostfix(const std::string & infix)
{
  std::string postfix;
  Stack< std::string > stack;
  size_t cntOfOpenBrackets = 0;
  for (size_t i = 0; i < infix.size(); ++i)
  {
    std::string tempstr;
    while (infix[i] != ' ' && i < infix.size())
    {
      tempstr += infix[i++];
    }
    if (isOperand(tempstr))
    {
      if (!postfix.empty())
      {
        postfix += ' ';
      }
      postfix += tempstr;
    }
    else if ((tempstr.size() == 1) && isOperator(tempstr))
    {
      if (stack.empty() || (tempstr.back() == '('))
      {
        if (tempstr.back() == '(')
        {
          ++cntOfOpenBrackets;
        }
        stack.push(tempstr);
      }
      else if (tempstr.back() == ')')
      {
        if (!cntOfOpenBrackets)
        {
          throw std::logic_error("non-correct infix expression, (!cntOfOpenBrackets)");
        }
        while (stack.top().back() != '(')
        {
          postfix += ' ';
          postfix += stack.top();
          stack.pop();
        }
        stack.pop();
        --cntOfOpenBrackets;
      }
      else if (isLoverPrecedence(stack.top(), tempstr))
      {
        stack.push(tempstr);
      }
      else if (isGreaterOrEqualPrecedence(stack.top(), tempstr))
      {
        postfix += ' ';
        postfix += stack.top();
        stack.pop();
        stack.push(tempstr);
      }
    }
    else
    {
      throw std::logic_error("non-correct infix expression");
    }
  }
  while (!stack.empty())
  {
    postfix += ' ';
    postfix += stack.top();
    stack.pop();
  }
  return postfix;
}
bool nikonov::isOperand(const std::string & el)
{
  try
  {
    size_t pos;
    std::stold(el, &pos);
    return (pos == el.size());
  }
  catch (const std::exception & e)
  {
    return false;
  }
}
bool nikonov::isOperator(const std::string & el)
{
  if (el.size() > 1 && el.empty())
  {
    return false;
  }
  bool flag = false;
  char ops[] = {'-', '+', '*', '/', '%', '(', ')'};
  for (auto op : ops)
  {
    if (el.back() == op)
    {
      flag = true;
    }
  }
  return flag;
}
int nikonov::getPrecedence(char el)
{
  if (el == '+' || el == '-')
  {
    return 2;
  }
  else if (el == '*'|| el == '/'|| el == '%')
  {
    return 3;
  }
  else if (el == '(' || el == ')')
  {
    return 0;
  }
  throw std::logic_error("ERROR: this op doesn't supported");
}
namespace
{
  bool isLoverPrecedence(const std::string & a, const std::string & b)
  {
   return !isGreaterOrEqualPrecedence(a, b);
  }
  bool isGreaterOrEqualPrecedence(const std::string & a, const std::string & b)
  {
    if (nikonov::getPrecedence(a.back()) >= nikonov::getPrecedence(b.back()))
    {
      return true;
    }
    return false;
  }
}

