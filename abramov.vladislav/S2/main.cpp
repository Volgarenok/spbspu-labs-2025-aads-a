#include <iostream>
#include <fstream>
#include "postfix_infix_exprs.hpp"

int main(int argc, char **argv)
{
  if (argc > 2)
  {
    std::cerr << "Wrong parameters\n";
    return 1;
  }
  try
  {
    if (argc == 2)
    {
      std::ifstream input(argv[1]);
      abramov::calcInfix(input);
    }
    else
    {
      abramov::calcInfix(std::cin);
    }
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what();
    return 1;
  }
}
