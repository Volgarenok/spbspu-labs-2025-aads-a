#include <iostream>
#include <fstream>
#include <sstream>
#include "command.h"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "missing filename\n";
    return 1;
  }

  std::ifstream infile(argv[1]);
  if (!infile.is_open())
  {
    std::cerr << "can't open file\n";
    return 1;
  }

  asafov::maps_t datasets;
  std::string line;

  while (std::getline(infile, line))
  {
    if (line.empty())
    {
      continue;
    }
    std::istringstream ss(line);
    std::string name;
    ss >> name;

    int key;
    std::string value;
    asafov::map_t dict;
    while (ss >> key >> value)
    {
      dict[key] = value;
    }
    datasets[name] = dict;
  }

  while (std::getline(std::cin, line))
  {
    if (!line.empty())
    {
      asafov::processCommand(datasets, line);
    }
  }

  return 0;
}
