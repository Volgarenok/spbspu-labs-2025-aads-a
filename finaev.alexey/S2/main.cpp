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
    std::istream& input = (argc == 2) ? (mainFile.open(argv[1]), mainFile) : std::cin;
    if (argc == 2 && !mainFile)
    {
      throw std::logic_error("error open file\n");
    }
    finaev::countFinalResults(input);
  }
  catch (const std::exception& e)
  {
    std::cout << e.what();
    return 1;
  }
}
