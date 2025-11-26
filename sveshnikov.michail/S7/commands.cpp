#include "commands.hpp"
#include <iostream>

namespace
{
  using Bounds_t = sveshnikov::AvlTree< std::string, sveshnikov::Array< unsigned int > >;

  void ioBound(const Bounds_t &bounds, std::ostream &out);

  void ioBound(const Bounds_t &bounds, std::ostream &out)
  {
    if (bounds.empty())
    {
      out << '\n';
    }
    for (auto i = bounds.begin(); i != bounds.end(); i++)
    {
      out << i->first;
      for (size_t j = 0; j < i->second.getSize(); j++)
      {
        out << ' ' << i->second[j];
      }
      out << '\n';
    }
  }
}

void sveshnikov::graphs(const GraphsMap_t &graph_map, std::ostream &out)
{
  if (graph_map.empty())
  {
    out << '\n';
    return;
  }
  AvlTree< std::string, char > names;
  for (auto it = graph_map.begin(); it != graph_map.end(); it++)
  {
    names[it->first];
  }
  for (auto it = names.begin(); it != names.end(); it++)
  {
    out << it->first << '\n';
  }
}

void sveshnikov::vertexes(const GraphsMap_t &graph_map, std::istream &in, std::ostream &out)
{
  std::string graph_name;
  in >> graph_name;
  Array< std::string > verts = graph_map.at(graph_name).get_vertexes();
  if (verts.empty())
  {
    out << '\n';
    return;
  }
  for (size_t i = 0; i < verts.getSize(); i++)
  {
    out << verts[i] << '\n';
  }
}

void sveshnikov::outbound(const GraphsMap_t &graph_map, std::istream &in, std::ostream &out)
{
  std::string graph_name, vertex;
  in >> graph_name >> vertex;
  Bounds_t outbounds = graph_map.at(graph_name).get_outbounds(vertex);
  ioBound(outbounds, out);
}

void sveshnikov::inbound(const GraphsMap_t &graph_map, std::istream &in, std::ostream &out)
{
  std::string graph_name, vertex;
  in >> graph_name >> vertex;
  Bounds_t inbounds = graph_map.at(graph_name).get_inbounds(vertex);
  ioBound(inbounds, out);
}

void sveshnikov::bind(GraphsMap_t &graph_map, std::istream &in)
{
  std::string graph_name, vertex_out, vertex_in;
  unsigned int weight = 0;
  in >> graph_name >> vertex_out >> vertex_in >> weight;
  graph_map.at(graph_name).bind(vertex_out, vertex_in, weight);
}

void sveshnikov::cut(GraphsMap_t &graph_map, std::istream &in)
{
  std::string graph_name, vertex_out, vertex_in;
  unsigned int weight = 0;
  in >> graph_name >> vertex_out >> vertex_in >> weight;
  graph_map.at(graph_name).cut(vertex_out, vertex_in, weight);
}

void sveshnikov::create(GraphsMap_t &graph_map, std::istream &in)
{
  std::string graph_name;
  in >> graph_name;
  if (graph_map.find(graph_name) != graph_map.end())
  {
    throw std::out_of_range("ERROR: this graph already exists!");
  }
  size_t count_k = 0;
  in >> count_k;
  Graph graph;
  for (size_t i = 0; i < count_k; i++)
  {
    std::string vertex;
    in >> vertex;
    graph.add_vertex(vertex);
  }
  graph_map[graph_name] = graph;
}

void sveshnikov::merge(GraphsMap_t &graph_map, std::istream &in)
{
  std::string new_graph, old_graph1, old_graph2;
  in >> new_graph >> old_graph1 >> old_graph2;
  Graph graph(graph_map.at(old_graph1), graph_map.at(old_graph2));
  graph_map[new_graph] = graph;
}

void sveshnikov::extract(GraphsMap_t &graph_map, std::istream &in)
{
  std::string new_graph_name, old_graph_name;
  size_t count_k = 0;
  in >> new_graph_name >> old_graph_name >> count_k;
  Graph graph(graph_map.at(old_graph_name));
  Array< std::string > verts = graph.get_vertexes();
  Graph new_graph;
  for (size_t i = 0; i < count_k; i++)
  {
    std::string vertex;
    in >> vertex;
    if (!graph.check_vert_existance(vertex))
    {
      throw std::out_of_range("ERROR: there are no corresponding vertexes in the graph!");
    }
    new_graph.add_vertex(vertex);
  }
  for (size_t i = 0; i < verts.getSize(); i++)
  {
    if (!new_graph.check_vert_existance(verts[i]))
    {
      graph.delete_vertex(verts[i]);
    }
  }
  graph_map[new_graph_name] = graph;
}
