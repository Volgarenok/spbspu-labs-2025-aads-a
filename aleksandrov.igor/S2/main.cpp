#include <iostream>
#include <fstream>
#include "queue.hpp"
#include "stack.hpp"
#include "infix-part.hpp"
#include "expr-utils.hpp"

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
  std::istream& input = file.is_open() ? file : std::cin;

  Stack< long long int > results;
  try
  {
    Queue< Queue< InfixPart > > infixExprs = getInfixExprs(input);
    Queue< Queue< InfixPart > > postfixExprs = infixesToPostfixes(infixExprs);
    results = evalPostfixExprs(postfixExprs);
  }
  catch (const std::exception& e)
  {
    std::cerr << "ERROR: " << e.what() << "\n";
    return 1;
  }

  if (!results.empty())
  {
    std::cout << results.drop();
  }
  while (!results.empty())
  {
    std::cout << " " << results.drop();
  }
  std::cout << "\n";
}

