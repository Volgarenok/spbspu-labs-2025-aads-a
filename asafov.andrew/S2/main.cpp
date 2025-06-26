#include <iostream>
#include <fstream>
#include <stdexcept>
#include "countFunctions.h"

int main(int argc, char* argv[])
{
  asafov::stack_num_t result;
  if (argc == 1)
  {
    try
    {
      asafov::count(result, std::cin);
    }
    catch (const std::exception& e)
    {
      std::cerr << e.what();
      return 1;
    }
  }
  else if (argc == 2)
  {
    std::ifstream fin;
    fin.open(argv[1]);
    try
    {
      asafov::count(result, fin);
    }
    catch (const std::exception& e)
    {
      std::cerr << e.what();
      return 1;
    }
  }
  else
  {
    std::cerr << "invalid arguments\n";
    return 1;
  }
}
