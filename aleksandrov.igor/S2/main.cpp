#include <iostream>
#include <fstream>
#include "queue.hpp"
#include "stack.hpp"
#include "expression-part.hpp"
#include "expression-utils.hpp"

int main(int argc, char* argv[])
{
  using namespace aleksandrov;

  if (argc > 2)
  {
    std::cerr << "ERROR: Too many arguments!\n";
    return 1;
  }

  std::ifstream file;
  if (argc == 2)
  {
    file.open(argv[1]);
    if (!file)
    {
      std::cerr << "ERROR: Could not open a file!\n";
      return 1;
    }
  }
  std::istream& in = file.is_open() ? file : std::cin;

  Stack< long long int > results;
  try
  {
    Queue< Queue< ExpressionPart > > exprs;
    getExpressions(in, exprs);

    Queue< Queue< ExpressionPart > > postfixes(getPostfixForms(exprs));

    results = evalPostfixExpressions(postfixes);
  }
  catch (const std::exception& e)
  {
    std::cerr << "ERROR: " << e.what() << '\n';
    return 1;
  }

  if (!results.empty())
  {
    std::cout << results.top();
    results.pop();
  }
  while (!results.empty())
  {
    std::cout << ' ' << results.top();
    results.pop();
  }
  std::cout << '\n';
}

