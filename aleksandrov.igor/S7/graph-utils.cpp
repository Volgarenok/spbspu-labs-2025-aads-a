#include "graph-utils.hpp"
#include <iostream>
#include <limits>
#include <functional>
#include "graph-commands.hpp"

void aleksandrov::readGraphs(std::istream& in, Graphs& graphs)
{
  std::string graphName;
  size_t edgesCount = 0;
  while (in >> graphName >> edgesCount)
  {
    Graph graph;
    for (size_t i = 0; i < edgesCount; ++i)
    {
      std::string from;
      std::string to;
      size_t weight = 0;
      if (!(in >> from >> to >> weight))
      {
        break;
      }
      graph.bind(from, to, weight);
    }
    if (in.fail())
    {
      in.clear();
    }
    else
    {
      graphs.insert({ graphName, graph });
    }
  }
}

void aleksandrov::processCommands(std::istream& in, std::ostream& out, Graphs& src)
{
  std::map< std::string, std::function< void() > > commands;
  commands["graphs"] = std::bind(graphs, std::cref(src), std::ref(out));
  commands["vertexes"] = std::bind(vertexes, std::cref(src), std::ref(in), std::ref(out));
  commands["outbound"] = std::bind(outBound, std::cref(src), std::ref(in), std::ref(out));
  commands["inbound"] = std::bind(inBound, std::cref(src), std::ref(in), std::ref(out));
  commands["bind"] = std::bind(bind, std::ref(src), std::ref(in));
  commands["cut"] = std::bind(cut, std::ref(src), std::ref(in));
  commands["create"] = std::bind(create, std::ref(src), std::ref(in));
  commands["merge"] = std::bind(merge, std::ref(src), std::ref(in));
  commands["extract"] = std::bind(extract, std::ref(src), std::ref(in));

  std::string command;
  while (in >> command)
  {
    try
    {
      commands.at(command)();
    }
    catch (const std::logic_error&)
    {
      out << "<INVALID COMMAND>\n";
      in.clear();
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}

