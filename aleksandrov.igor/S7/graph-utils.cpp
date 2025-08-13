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
      size_t weight;
      if (!(in >> from >> to >> weight))
      {
        in.clear();
        return;
      }
      if (graph.vertices.find(from) == graph.vertices.end())
      {
        graph.vertices[from].name = from;
      }
      if (graph.vertices.find(to) == graph.vertices.end())
      {
        graph.vertices[to].name = to;
      }
      graph.vertices[from].adjacentyList.push_back({ to, weight });
    }
    graph.name = graphName;
    graphs[graphName] = graph;
  }
}

void aleksandrov::processCommands(std::istream& in, std::ostream& out, Graphs& src)
{
  std::map< std::string, std::function< void() > > commands;
  commands["graphs"] = std::bind(graphs, std::cref(src), std::ref(out));
  commands["vertexes"] = std::bind(vertexes, std::cref(src), std::ref(in), std::ref(out));
  commands["outbound"] = std::bind(outbound, std::cref(src), std::ref(in), std::ref(out));
  commands["inbound"] = std::bind(inbound, std::cref(src), std::ref(in), std::ref(out));

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
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      in.clear();
    }
  }
}

