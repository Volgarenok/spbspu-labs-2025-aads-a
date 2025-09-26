#include <iostream>
#include <fstream>
#include <limits>
#include "graph.hpp"
#include "commands.hpp"

namespace
{
  void inputGraphs(std::istream& in, std::map< std::string, krylov::Graph >& graphs)
  {
    std::string name;
    size_t count = 0;
    while (in >> name >> count)
    {
      krylov::Graph gr;
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

int main(int argc, char** argv)
{
  using namespace krylov;
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
  std::map< std::string, krylov::Graph > graphs;
  inputGraphs(file, graphs);
  std::map< std::string, std::function< void() > > commands;
  commands["graphs"] = std::bind(printGraphs, std::ref(std::cout), std::cref(graphs));
  commands["vertexes"] = std::bind(vertexes, std::ref(std::cin), std::ref(std::cout), std::cref(graphs));
  commands["outbound"] = std::bind(outBound, std::ref(std::cin), std::ref(std::cout), std::cref(graphs));
  commands["inbound"] = std::bind(inBound, std::ref(std::cin), std::ref(std::cout), std::cref(graphs));
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
    catch(const std::exception& e)
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
