#include <iostream>
#include <fstream>
#include "convertExpressions.hpp"

int main(int argc, char * argv[])
{
  try
  {
    if (argc > 2)
    {
      std::cerr << "too much arguments\n";
      return 1;
    }
    std::ifstream mainFile;
    if (argc == 2)
    {
      mainFile.open(argv[1]);
      if (!mainFile)
      {
        throw std::logic_error("error main file open\n");
      }
    }
    if (mainFile.is_open())
    {
      finaev::countFinalResults(mainFile);
    }
    else
    {
      finaev::countFinalResults(std::cin);
    }
  }
  catch (const std::exception& e)
  {
    std::cout << e.what();
    return 1;
  }
}
