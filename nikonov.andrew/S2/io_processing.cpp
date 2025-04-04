#include "io_processing.hpp"
nikonov::Queue< std::string > nikonov::readInfixExpressions(std::istream & in)
{
  Queue< std::string > que;
  std::string infix;
  std::noskipws(in);
  while (!in.eof())
  {
    std::getline(in, infix);
    if (!infix.empty())
    {
      que.push(infix);
    }
  }
  std::skipws(in);
  return que;
}
void nikonov::printResults(Stack< std::string > toOut, std::ostream & out)
{
  if (toOut.empty())
  {
    return;
  }
  out << toOut.top() << ' ';
  toOut.pop();
  for (size_t i = 0; i < toOut.size(); ++i)
  {
    out << toOut.top() << ' ';
    toOut.pop();
  }
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
      postfix += tempstr;
      postfix += ' ';
    }
    else if (isOperator(tempstr))
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
          throw std::logic_error("non-correct infix expression");
        }
        while (stack.top().back() != '(')
        {
          postfix += stack.top();
          postfix += ' ';
          stack.pop();
        }
        stack.pop();
        --cntOfOpenBrackets;
      }
      else if (isLoverPrecedence(stack.top(), tempstr))
      {
        postfix += stack.top();
        postfix += ' ';
        stack.pop();
        stack.push(tempstr);
      }
      else if (!isLoverPrecedence(stack.top(), tempstr))
      {
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
    postfix += stack.top();
    postfix += ' ';
    stack.pop();
  }
  postfix.pop_back(); //пока костыль
  return postfix;
}
bool nikonov::isOperand(const std::string & el)
{
  try
  {
    std::stold(el);
    return true;
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
  else if (el.back() == '+' || el.back() == '-' || el.back() == '*' || el.back() == '/' || el.back() == '&')
  {
    return true;
  }
  return false;
}
bool nikonov::isLoverPrecedence(const std::string & a, const std::string & b)
{
  if ((a.back() == '-' || a.back() == '+') && (b.back() == '/' || b.back() == '*' || b.back() == '&'))
  {
    return true;
  }
  return false;
}
