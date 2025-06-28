#include <iostream>
#include <fstream>
#include <string>
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

    size_t pos = 0;
    while (pos < line.size() && std::isspace(line[pos]))
    {
      ++pos;
    }
    size_t start = pos;
    while (pos < line.size() && !std::isspace(line[pos]))
    {
      ++pos;
    }
    std::string name = line.substr(start, pos - start);
    asafov::map_t dict;

    while (pos < line.size())
    {
      while (pos < line.size() && std::isspace(line[pos]))
      {
        ++pos;
      }
      if (pos >= line.size())
      {
        break;
      }

      start = pos;
      while (pos < line.size() && !std::isspace(line[pos]))
      {
        ++pos;
      }
      int key = std::stoi(line.substr(start, pos - start));

      while (pos < line.size() && std::isspace(line[pos]))
      {
        ++pos;
      }
      if (pos >= line.size())
      {
        break;
      }

      start = pos;
      while (pos < line.size() && !std::isspace(line[pos]))
      {
        ++pos;
      }
      std::string value = line.substr(start, pos - start);

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
