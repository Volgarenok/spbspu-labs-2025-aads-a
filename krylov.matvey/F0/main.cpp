#include <iostream>
#include <fstream>
#include <limits>
#include "commands.hpp"

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cerr << "Incorrect parameters!\n";
    return 1;
  }

  krylov::CommandProcessor processor(std::cout);
  for (int i = 1; i < argc; i++)
  {
    std::ifstream file(argv[i]);
    if (!file)
    {
      std::cerr << "Incorrect file!\n";
      return 1;
    }

    std::string line;
    while (std::getline(file, line))
    {
      try
      {
        processor.execute(line);
      }
      catch (const std::exception& e)
      {
        std::cout << "<INVALID COMMAND>\n";
        file.clear();
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }
  }
}
