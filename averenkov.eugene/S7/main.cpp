#include <iostream>
#include <fstream>
#include <functional>
#include <stdexcept>
#include <limits>
#include <graph.hpp>
#include <BiTree.hpp>
#include "commands.hpp"

int main(int argc, char* argv[])
{
  using namespace averenkov;
  if (argc != 2)
  {
    std::cerr << "Error\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "Error opening file\n";
    return 1;
  }

  Tree< std::string, Graph > graphs;
  loadGraphsFromFile(graphs, file);

  Tree< std::string, std::function< void() > > cmds;
  commandsInit(cmds, graphs);

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      cmds.at(command)();
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}

