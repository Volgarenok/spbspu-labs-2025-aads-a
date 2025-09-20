#include <iostream>
#include <fstream>
#include <limits>
#include "graph.hpp"
#include "commands.hpp"

namespace
{
  void inputGraphs(std::istream& in, finaev::AVLtree< std::string, finaev::Graph >& graphs)
  {
    std::string name;
    size_t count = 0;
    while (in >> name >> count)
    {
      finaev::Graph gr;
      for (size_t i = 0; i < count; ++i)
      {
        std::string vect1, vect2;
        size_t weight = 0;
        in >> vect1 >> vect2 >> weight;
        gr.addEdge(vect1, vect2, weight);
      }
      graphs[name] = gr;
    }
  }
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Uncorrect input\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "Error openning\n";
    return 1;
  }
  finaev::AVLtree< std::string, finaev::Graph > graphs;
  inputGraphs(file, graphs);
  auto commands = finaev::createCommandsHandler(std::cin, std::cout, graphs);
  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      commands.at(cmd)();
    }
    catch(...)
    {
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
