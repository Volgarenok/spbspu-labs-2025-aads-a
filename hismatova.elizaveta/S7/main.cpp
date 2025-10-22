#include <iostream>
#include <fstream>
#include <string>
#include "graph.h"

int main(int argc, char* argv[])
{
  using namespace hismatova;
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " filename\n";
    return 1;
  }

  std::string filename = argv[1];
  GraphProcessor processor;

  processor.loadGraphsFromFile(filename);

  std::string command;
  while (std::getline(std::cin, command))
  {
    if (command.empty())
    {
      continue;
    }
    processor.executeCommand(command);
  }

  return 0;
}
