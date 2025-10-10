#include <iostream>
#include <fstream>
#include <limits>
#include "graph.hpp"
#include "commands.hpp"
#include "inpututil.hpp"

int main(int argc, char* argv[])
{
  using namespace kushekbaev;
  if (argc != 2)
  {
    std::cerr << "IVALID INPUT\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "INVALID FILE\n";
    return 1;
  }
  std::map< std::string, Graph > graphs;
  inputGraphs(file, graphs);
  std::map< std::string, std::function< void() > > commands;
  commands["graphs"] = std::bind(print_graphs, std::ref(std::cout), std::cref(graphs));
  commands["vertexes"] = std::bind(vertexes, std::ref(std::cin), std::ref(std::cout), std::cref(graphs));
  commands["outbound"] = std::bind(out_bound, std::ref(std::cin), std::ref(std::cout), std::cref(graphs));
  commands["inbound"] = std::bind(in_bound, std::ref(std::cin), std::ref(std::cout), std::cref(graphs));
  commands["bind"] = std::bind(bind, std::ref(std::cin), std::ref(graphs));
  commands["cut"] = std::bind(cut, std::ref(std::cin), std::ref(graphs));
  commands["merge"] = std::bind(merge, std::ref(std::cin), std::ref(graphs));
  commands["create"] = std::bind(create, std::ref(std::cin), std::ref(graphs));
  commands["extract"] = std::bind(extract, std::ref(std::cin), std::ref(graphs));
  std::string command;
  while (std::cin >> command)
  {
    try
    {
      commands.at(command)();
    }
    catch (const std::exception&)
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
