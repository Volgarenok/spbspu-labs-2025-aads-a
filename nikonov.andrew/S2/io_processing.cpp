#include "io_processing.hpp"
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
void nikonov::printResults(Stack< Postfix > toOut, std::ostream & out)
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
