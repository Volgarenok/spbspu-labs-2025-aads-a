#include <iostream>
#include <fstream>
#include "processMathExpressions.hpp"

int main(int argc, char** argv)
{
  try
  {
    if (argc > 2)
    {
      std::cerr << "Incorrect arguments\n";
      return 1;
    }
    else if (argc == 2)
    {
      std::ifstream input(argv[1]);
      krylov::calculateExpression(input);
    }
    else
    {
      krylov::calculateExpression(std::cin);
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
}
