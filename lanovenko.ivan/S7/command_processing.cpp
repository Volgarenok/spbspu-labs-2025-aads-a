#include "command_processing.hpp"

namespace
{
  void print_bounds(std::ostream& out, const std::vector< std::pair< std::string, std::vector< int > > >& bounds)
  {
    if (bounds.empty())
    {
      out << '\n';
    }
    for (auto it = bounds.cbegin(); it != bounds.cend(); it++)
    {
      out << it->first;
      for (auto itWeight = it->second.cbegin(); itWeight != it->second.cend(); itWeight++)
      {
        out << ' ' << *itWeight;
      }
      out << '\n';
    }
  }
}

void lanovenko::get_graphs(std::ostream& out, const map& graphs)
{
  if (graphs.empty())
  {
    out << '\n';
  }
  for (auto it = graphs.cbegin(); it != graphs.cend(); it++)
  {
    out << it->first << '\n';
  }
}

void lanovenko::get_vertexes(std::istream& in, std::ostream& out, const map& graphs)
{
  std::string graphName;
  in >> graphName;
  const Graph& graph = graphs.at(graphName);
  std::vector< std::string > vertexes = graph.get_vertexes();
  if (vertexes.empty())
  {
    out << '\n';
  }
  for (auto it = vertexes.begin(); it != vertexes.end(); it++)
  {
    out << *it << '\n';
  }
}

void lanovenko::get_outbound(std::istream& in, std::ostream& out, const map& graphs)
{
  std::string graphName, vertexName;
  in >> graphName >> vertexName;
  const Graph& graph = graphs.at(graphName);
  if (!graph.has_vertex(vertexName))
  {
    throw std::logic_error("<THERE IS NO SUCH VERTEX>");
  }
  auto outbounds = graph.get_outbound(vertexName);
  print_bounds(out, outbounds);
}

void lanovenko::get_inbound(std::istream& in, std::ostream& out, const map& graphs)
{
  std::string graphName, vertexName;
  in >> graphName >> vertexName;
  const Graph& graph = graphs.at(graphName);
  if (!graph.has_vertex(vertexName))
  {
    throw std::logic_error("<THERE IS NO SUCH VERTEX>");
  }
  auto inbounds = graph.get_inbound(vertexName);
  print_bounds(out, inbounds);
}

void lanovenko::create_edge(std::istream& in, map& graphs)
{
  std::string graphName, v1, v2;
  int weight = 0;
  in >> graphName >> v1 >> v2 >> weight;
  Graph& graph = graphs.at(graphName);
  graph.add_edge(v1, v2, weight);
}

void lanovenko::delete_edge(std::istream& in, map& graphs)
{
  std::string graphName, v1, v2;
  int weight = 0;
  in >> graphName >> v1 >> v2 >> weight;
  Graph& graph = graphs.at(graphName);
  graph.delete_edge(v1, v2, weight);
}

void lanovenko::create_graph(std::istream& in, map& graphs)
{
  std::string graphName;
  in >> graphName;
  if (graphs.find(graphName) != graphs.end())
  {
    throw std::logic_error("<SUCH A GRAPH ALREADY EXISTS>");
  }
  Graph newGraph;
  size_t vertexesCount = 0;
  in >> vertexesCount;
  std::string vertex;
  for (size_t i = 0; i < vertexesCount; i++)
  {
    in >> vertex;
    newGraph.add_vertex(vertex);
  }
  graphs[graphName] = newGraph;
}

void lanovenko::merge_graph(std::istream& in, map& graphs)
{
  std::string newGraphName, graphName1, graphName2;
  in >> newGraphName >> graphName1 >> graphName2;
  if (graphs.find(newGraphName) != graphs.end())
  {
    throw std::logic_error("< SUCH A GRAPH ALREADY EXISTS >");
  }
  const Graph& graph1 = graphs.at(graphName1);
  const Graph& graph2 = graphs.at(graphName2);
  Graph mergedGraph = graph1;
  mergedGraph.merge(graph2);
  graphs[newGraphName] = mergedGraph;
}

void lanovenko::extract_graph(std::istream& in, map& graphs)
{
  std::string newGraphName, graphName;
  in >> newGraphName >> graphName;
  if (graphs.find(newGraphName) != graphs.end())
  {
    throw std::logic_error("< SUCH A GRAPH ALREADY EXISTS >");
  }
  const Graph& graph = graphs.at(graphName);
  size_t countVertexes = 0;
  in >> countVertexes;
  std::set< std::string > vertexes;
  std::string vertex;
  for (size_t i = 0; i < countVertexes; ++i)
  {
    in >> vertex;
    graph.has_vertex(vertex);
    vertexes.insert(vertex);
  }
  Graph extractedGraph;
  extractedGraph.extract(graph, vertexes);
  graphs[newGraphName] = extractedGraph;
}
