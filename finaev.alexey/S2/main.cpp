#include <iostream>
#include "convertExpressions.hpp"
#include <fstream>


int main(int argc, char * argv[])
{
  try
  {
    if (argc > 2)
    {
      std::cerr << "too much arguments\n";
      return 1;
    }
    else if (argc == 2)
    {
      std::ifstream in(argv[1]);
      finaev::countFinalResults(in);
    }
    else
    {
      finaev::countFinalResults(std::cin);
    }
  }
  catch (const std::logic_error& e)
  {
    std::cout << e.what();
    return 1;
  }
  catch (const std::overflow_error& e)
  {
    std::cout << e.what();
    return 1;
  }
}
