#include <iostream>
#include <fstream>
#include <limits>
#include <cstddef>
#include <map>
#include <functional>
#include "hash_table.hpp"
#include "AVLtree.hpp"
#include "commands.hpp"
#include "graph.hpp"

namespace
{
  tkach::AvlTree< std::string, tkach::Graph > inputDataSets(std::istream& in)
  {
    using namespace tkach;
    AvlTree< std::string, Graph > fulldata;
    while(in)
    {
      std::string name;
      size_t count = 0;
      Graph temp;
      if (in >> name >> count)
      {
        for (size_t i = 0; i < count; ++ i)
        {
          size_t weight = 0;
          std::string vert1, vert2;
          if (in >> vert1 >> vert2 >> weight)
          {
            temp.addEdge(vert1, vert2, weight);
          }
        }
        fulldata[name] = temp;
      }
    }
    return fulldata;
  }
}

int main(const int argc, const char* const* const argv)
{
  using namespace tkach;
  if (argc != 2)
  {
    std::cerr << "Error: incorrect input\n";
    return 1;
  }
  std::fstream in(argv[1]);
  if (!in.is_open())
  {
    std::cerr << "File is not open\n";
    return 1;
  }
  using namespace std::placeholders;
  AvlTree< std::string, Graph > data = inputDataSets(in);
  if (!in.eof())
  {
    std::cerr << "Error: incorrect input\n";
    return 1;
  }
  std::map< std::string, std::function< void() > > cmds;
  cmds["graphs"] = std::bind(printGraphs, std::ref(std::cout), std::cref(data));
  cmds["vertexes"] = std::bind(printVertexes, std::ref(std::cout), std::ref(std::cin), std::cref(data));
  cmds["outbound"] = std::bind(printOutbound, std::ref(std::cout), std::ref(std::cin), std::cref(data));
  cmds["inbound"] = std::bind(printInbound, std::ref(std::cout), std::ref(std::cin), std::cref(data));
  cmds["bind"] = std::bind(bind, std::ref(std::cin), std::ref(data));
  cmds["cut"] = std::bind(cut, std::ref(std::cin), std::ref(data));
  cmds["merge"] = std::bind(merge, std::ref(std::cin), std::ref(data));
  cmds["create"] = std::bind(create, std::ref(std::cin), std::ref(data));
  cmds["extract"] = std::bind(extract, std::ref(std::cin), std::ref(data));
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)();
    }
    catch (const std::exception&)
    {
      std::cout << "<INVALID COMMAND>" << "\n";
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
  return 0;
}
