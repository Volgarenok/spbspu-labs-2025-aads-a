#include <iostream>
#include <fstream>
#include <limits>
#include <functional>
#include <tree.hpp>
#include "graph.hpp"
#include "commands.hpp"

namespace
{
  void inputGraphs(std::istream & in, mozhegova::BiTree< std::string, mozhegova::Graph > & graphs)
  {
    std::string graphname;
    size_t edgecount = 0;
    while (in >> graphname >> edgecount)
    {
      mozhegova::Graph temp;
      for (size_t i = 0; i < edgecount; ++i)
      {
        std::string v1, v2;
        size_t weight = 0;
        in >> v1 >> v2 >> weight;
        temp.addEdge(v1, v2, weight);
      }
      graphs[graphname] = temp;
    }
  }
}

int main(int argc, char * argv[])
{
  using namespace mozhegova;
  if (argc != 2)
  {
    std::cerr << "wrong arguments\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "file is not open\n";
    return 1;
  }
  BiTree< std::string, Graph > graphs;
  inputGraphs(file, graphs);

  BiTree< std::string, std::function< void() > > cmds;
  cmds["graphs"] = std::bind(printGraphs, std::ref(std::cout), std::cref(graphs));
  cmds["vertexes"] = std::bind(printVertexes, std::ref(std::cin), std::ref(std::cout), std::cref(graphs));
  cmds["outbound"] = std::bind(printOutbounds, std::ref(std::cin), std::ref(std::cout), std::cref(graphs));
  cmds["inbound"] = std::bind(printInbounds, std::ref(std::cin), std::ref(std::cout), std::cref(graphs));
  cmds["bind"] = std::bind(bind, std::ref(std::cin), std::ref(graphs));
  cmds["cut"] = std::bind(cut, std::ref(std::cin), std::ref(graphs));
  cmds["merge"] = std::bind(merge, std::ref(std::cin), std::ref(graphs));
  cmds["create"] = std::bind(create, std::ref(std::cin), std::ref(graphs));
  cmds["extract"] = std::bind(extract, std::ref(std::cin), std::ref(graphs));

  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)();
    }
    catch (const std::exception &)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
}
