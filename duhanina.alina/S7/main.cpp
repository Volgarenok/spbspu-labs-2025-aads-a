#include <iostream>
#include <fstream>
#include <functional>
#include <stdexcept>
#include <limits>
#include "graph.hpp"
#include "commands.hpp"

int main(int argc, char* argv[])
{
  using namespace duhanina;
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

  MapOfGraphs graphs;
  inputGraphs(file, graphs);

  Tree< std::string, std::function< void() >, std::less< std::string > > cmds;
  cmds["graphs"] = std::bind(printGraphsNames, std::ref(std::cout), std::cref(graphs));
  cmds["vertexes"] = std::bind(printVertexesNames, std::ref(std::cout), std::ref(std::cin), std::cref(graphs));
  cmds["outbound"] = std::bind(printOutbounds, std::ref(std::cout), std::ref(std::cin), std::cref(graphs));
  cmds["inbound"] = std::bind(printInbounds, std::ref(std::cout), std::ref(std::cin), std::cref(graphs));
  cmds["bind"] = std::bind(bind, std::ref(std::cin), std::ref(graphs));
  cmds["cut"] = std::bind(cut, std::ref(std::cin), std::ref(graphs));
  cmds["create"] = std::bind(create, std::ref(std::cin), std::ref(graphs));
  cmds["merge"] = std::bind(merge, std::ref(std::cin), std::ref(graphs));
  cmds["extract"] = std::bind(extract, std::ref(std::cin), std::ref(graphs));

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
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
  return 0;
}
