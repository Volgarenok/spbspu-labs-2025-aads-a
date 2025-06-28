#include <fstream>
#include <iostream>
#include <limits>
#include <tree/tree.hpp>
#include "graph.hpp"
#include "commands.hpp"

namespace
{
  using MapOfGraphs = demehin::Tree< std::string, demehin::Graph >;

  void inputGraphs(std::istream& in, MapOfGraphs& graphs)
  {
    std::string name;
    while (in >> name)
    {
      size_t cnt;
      demehin::Graph gr;
      in >> cnt;
      for (size_t i = 0; i < cnt; i++)
      {
        std::string from, to;
        unsigned int weight;
        in >> from >> to >> weight;
        gr.addEdge(from, to, weight);
      }
      graphs[name] = gr;
    }
  }
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    return 1;
  }

  std::ifstream file(argv[1]);
  MapOfGraphs mapOfGraphs;
  inputGraphs(file, mapOfGraphs);

  demehin::Tree< std::string, std::function< void() > > cmds;
  cmds["graphs"] = std::bind(demehin::printGraphsNames, std::ref(std::cout), std::cref(mapOfGraphs));
  cmds["vertexes"] = std::bind(demehin::printVertexesNames, std::ref(std::cout), std::ref(std::cin), std::cref(mapOfGraphs));
  cmds["outbound"]  = std::bind(demehin::printOutbounds, std::ref(std::cout), std::ref(std::cin), std::cref(mapOfGraphs));
  cmds["inbound"] = std::bind(demehin::printInbounds, std::ref(std::cout), std::ref(std::cin), std::cref(mapOfGraphs));
  cmds["bind"] = std::bind(demehin::bind, std::ref(std::cin), std::ref(mapOfGraphs));
  cmds["cut"] = std::bind(demehin::cut, std::ref(std::cin), std::ref(mapOfGraphs));
  cmds["create"] = std::bind(demehin::create, std::ref(std::cin), std::ref(mapOfGraphs));
  cmds["merge"] = std::bind(demehin::merge, std::ref(std::cin), std::ref(mapOfGraphs));
  cmds["extract"] = std::bind(demehin::extract, std::ref(std::cin), std::ref(mapOfGraphs));

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      cmds.at(command)();
    }
    catch (const std::logic_error&)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
