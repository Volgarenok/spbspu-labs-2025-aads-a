#include <fstream>
#include <functional>
#include <iostream>
#include <limits>

#include "commands.hpp"
#include "graph.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "Error in parameters. Run as ./lab <filename>\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "Error opening file\n";
    return 1;
  }

  using namespace zholobov;
  MapOfGraphs graphs;
  inputGraphs(file, graphs);

  zholobov::Tree< std::string, std::function< void() > > cmds;
  cmds["graphs"] = std::bind(cmdPrintGraphsNames, std::ref(std::cout), std::cref(graphs));
  cmds["vertexes"] = std::bind(cmdPrintVertexesNames, std::ref(std::cout), std::ref(std::cin), std::cref(graphs));
  cmds["outbound"] = std::bind(cmdPrintOutbounds, std::ref(std::cout), std::ref(std::cin), std::cref(graphs));
  cmds["inbound"] = std::bind(cmdPrintInbounds, std::ref(std::cout), std::ref(std::cin), std::cref(graphs));
  cmds["bind"] = std::bind(cmdBind, std::ref(std::cin), std::ref(graphs));
  cmds["cut"] = std::bind(cmdCut, std::ref(std::cin), std::ref(graphs));
  cmds["create"] = std::bind(cmdCreate, std::ref(std::cin), std::ref(graphs));
  cmds["merge"] = std::bind(cmdMerge, std::ref(std::cin), std::ref(graphs));
  cmds["extract"] = std::bind(cmdExtract, std::ref(std::cin), std::ref(graphs));

  std::string command;
  while (std::cin >> command) {
    try {
      cmds.at(command)();
      if (!std::cin) {
        std::cout << "<INVALID COMMAND>\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    } catch (...) {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
