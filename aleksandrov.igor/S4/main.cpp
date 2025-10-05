#include <iostream>
#include <fstream>
#include <limits>
#include "dataset-utils.hpp"

int main(int argc, char* argv[])
{
  using namespace aleksandrov;

  if (argc != 2)
  {
    std::cerr << "ERROR: Incorrect arguments!\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "ERROR: Incorrect file!\n";
    return 1;
  }

  Datasets datasets;
  try
  {
    readDatasets(file, datasets);

    std::string command;
    while (std::cin >> command)
    {
      processCommand(command, std::cin, std::cout, datasets);
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cin.clear();
    }
  }
  catch (const std::bad_alloc&)
  {
    std::cerr << "ERROR: Out of memory!\n";
    return 2;
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 2;
  }
}

