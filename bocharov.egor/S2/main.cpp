#include <exception>
#include <fstream>
#include <iostream>
#include "actions.hpp"

int main(int argc, char * argv[])
{
  try
  {
    bocharov::queue infixExprs;
    if (argc > 1)
    {
      std::ifstream file(argv[1]);
      bocharov::inputExprs(file, infixExprs);
    }
    else
    {
      bocharov::inputExprs(std::cin, infixExprs);
    }
    bocharov::queue postfixExprs = bocharov::convertExpr(infixExprs);
    bocharov::stackNumber results = bocharov::calculationExpr(postfixExprs);
    bocharov::outputResults(std::cout, results) << "\n";
  }
  catch (std::exception & e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
