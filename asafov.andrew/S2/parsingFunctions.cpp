#include "parsingFunctions.h"
#include "stack.hpp"
#include "queue.hpp"

std::string asafov::into_polish(std::string n)
{
  asafov::queue<char> in;
  for (const char sym : n) in.push(sym);

  asafov::queue<char> out;
  asafov::stack<char> stack;

  while (in.size() > 0)
  {
    if (in.back() == '(')
    {
      stack.push(in.drop());
    }
    else if (in.back() == ')')
    {
      
    }
    else if (in.back() == '+')
    {
      
    }
    else if (in.back() == '*')
    {
      
    }
    else if (in.back() == '-')
    {
      
    }
    else if (in.back() == '/')
    {
      
    }
    else if (in.back() == '%')
    {
      
    }
    else
    {
      
    }
  }

  std::string s;
  for (size_t i = 0; out.size() > 0; i++)
  {
    s[i] = out.drop();
  }
  return s;
}
