#ifndef PRINT_HPP
#define PRINT_HPP

#include <iostream>
#include "stack.hpp"

namespace shramko
{
  void outputStack(std::ostream& out, Stack<long long>& stack)
  {
    if (stack.empty())
    {
      return;
    }
    out << stack.top();
    stack.pop();
    while (!stack.empty())
    {
      out << " " << stack.top();
      stack.pop();
    }
  }
}

#endif
