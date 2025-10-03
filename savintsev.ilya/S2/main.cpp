#include <iostream>
#include <fstream>
#include "postfix-utils.h"

int main(int argc, char ** argv)
{
  if (argc > 2)
  {
    std::cerr << "ERROR: Too many arguments\n";
    return 1;
  }

  std::istream * input = &std::cin;
  std::ifstream file;

  if (argc == 2)
  {
    file.open(argv[1]);
    if (!file)
    {
      std::cerr << "ERROR: invalid file\n";
      return 1;
    }
    input = &file;
  }

  try
  {
    savintsev::evaluate_postfix_stream(*input);
  }
  catch (const std::exception & e)
  {
    std::cerr << e.what() << '\n';
    return 2;
  }

  return 0;
}
