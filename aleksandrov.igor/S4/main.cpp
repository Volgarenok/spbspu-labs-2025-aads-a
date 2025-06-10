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

  DatasetCollection datasets = readDatasets(file);

  std::string command;
  while (std::cin >> command)
  {
    processCommand(command, std::cin, datasets);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
}

