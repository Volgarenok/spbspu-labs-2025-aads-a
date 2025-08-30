#include <exception>
#include <fstream>
#include <iostream>
#include "actions.hpp"

int main(int argc, char * argv[])
{
  using namespace bocharov;
  try
  {
    queue infixExprs;
    if (argc > 1)
    {
      std::ifstream file(argv[1]);
      inputExprs(file, infixExprs);
    }
    else
    {
      inputExprs(std::cin, infixExprs);
    }
    queue postfixExprs = bocharov::convertExpr(infixExprs);
    stackNumber results = bocharov::calculationExpr(postfixExprs);
    outputResults(std::cout, results) << "\n";
  }
  catch (const std::exception & e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
