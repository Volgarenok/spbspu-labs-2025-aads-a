#include <iostream>
#include <fstream>
#include "postfix.hpp"
#include "io-infix.hpp"

int main(int argc, char **argv)
{
  using namespace sveshnikov;
  Queue< Postfix > infix_exprs;
  Stack< long long > results;
  try
  {
    if (argc == 1)
    {
      infix_exprs = inputInfix(std::cin);
    }
    else
    {
      std::ifstream in(argv[1]);
      infix_exprs = inputInfix(in);
    }
    while (!infix_exprs.empty())
    {
      results.push(infix_exprs.front().calculate());
      infix_exprs.pop();
    }
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  if (!results.empty())
  {
    std::cout << results.top();
    results.pop();
    while (!results.empty())
    {
      std::cout << " " << results.top();
      results.pop();
    }
  }
  std::cout << '\n';
  return 0;
}
