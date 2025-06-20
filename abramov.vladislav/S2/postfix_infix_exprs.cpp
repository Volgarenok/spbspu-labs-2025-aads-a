#include "postfix_infix_exprs.hpp"
#include <iostream>
#include <string>
#include "stack.hpp"
#include "queue.hpp"
#include "calcExprs.hpp"

std::istream &abramov::calcInfix(std::istream &in)
{
  std::string s;
  abramov::Stack< long long int > stack;
  while (in)
  {
    std::getline(in, s);
    if (s.empty())
    {
      continue;
    }
    stack.push(calcExpr(s));
  }
  if (stack.empty())
  {
    std::cout << "\n";
    return in;
  }
  std::cout << stack.top();
  stack.pop();
  while (!stack.empty())
  {
    std::cout << " " << stack.top();
    stack.pop();
  }
  std::cout << "\n";
  return in;
}
