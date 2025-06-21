#include "commands.hpp"
#include <vector/definition.hpp>
#include <tree/definition.hpp>
#include <hash_table/definition.hpp>

namespace {
  void print_vector(const maslevtsov::Vector< unsigned >& vector, std::ostream& out)
  {
    if (vector.empty()) {
      return;
    }
    out << *vector.cbegin();
    for (auto i = ++vector.cbegin(); i != vector.cend(); ++i) {
      out << ' ' << *i;
    }
  }
}

void maslevtsov::print_graphs(const graphs_map_t& graphs, std::ostream& out)
{
  Tree< std::string, int > names;
  for (auto i = graphs.cbegin(); i != graphs.cend(); ++i) {
    names[i->first];
  }
  for (auto i = names.cbegin(); i != names.cend(); ++i) {
    out << i->first << '\n';
  }
  if (names.empty()) {
    out << '\n';
  }
}

void maslevtsov::print_vertices(const graphs_map_t& graphs, std::istream& in, std::ostream& out)
{
  std::string graph_name;
  in >> graph_name;
  Graph graph = graphs.at(graph_name);
  Tree< std::string, int > names = graph.get_vertices();
  for (auto i = names.cbegin(); i != names.cend(); ++i) {
    out << i->first << '\n';
  }
  if (names.empty()) {
    out << '\n';
  }
}

void maslevtsov::print_outbound(const graphs_map_t& graphs, std::istream& in, std::ostream& out)
{
  std::string graph_name, vertice_name;
  in >> graph_name >> vertice_name;
  Graph graph = graphs.at(graph_name);
  if (!graph.check_vertice_existence(vertice_name)) {
    throw std::invalid_argument("non-existing vertice given");
  }
  Tree< std::string, Vector< unsigned > > outbound = graph.get_outbound(vertice_name);
  for (auto i = outbound.cbegin(); i != outbound.cend(); ++i) {
    out << i->first << ' ';
    print_vector(i->second, out);
    out << '\n';
  }
  if (outbound.empty()) {
    out << '\n';
  }
}

void maslevtsov::print_inbound(const graphs_map_t& graphs, std::istream& in, std::ostream& out)
{
  std::string graph_name, vertice_name;
  in >> graph_name >> vertice_name;
  Graph graph = graphs.at(graph_name);
  if (!graph.check_vertice_existence(vertice_name)) {
    throw std::invalid_argument("non-existing vertice given");
  }
  Tree< std::string, Vector< unsigned > > inbound = graph.get_inbound(vertice_name);
  for (auto i = inbound.cbegin(); i != inbound.cend(); ++i) {
    out << i->first << ' ';
    print_vector(i->second, out);
    out << '\n';
  }
  if (inbound.empty()) {
    out << '\n';
  }
}

void maslevtsov::bind_vertices(graphs_map_t& graphs, std::istream& in)
{
  std::string graph_name, vertice1, vertice2;
  unsigned weight = 0;
  in >> graph_name >> vertice1 >> vertice2 >> weight;
  graphs.at(graph_name).bind(vertice1, vertice2, weight);
}

void maslevtsov::cut_vertices(graphs_map_t& graphs, std::istream& in)
{
  std::string graph_name, vertice1, vertice2;
  unsigned weight = 0;
  in >> graph_name >> vertice1 >> vertice2 >> weight;
  graphs.at(graph_name).cut(vertice1, vertice2, weight);
}

void maslevtsov::create_graph(graphs_map_t& graphs, std::istream& in)
{
  std::string graph_name;
  in >> graph_name;
  if (graphs.find(graph_name) != graphs.end()) {
    throw std::invalid_argument("graph already exsist");
  }
  size_t edges_count = 0;
  in >> edges_count;
  Graph graph;
  for (size_t i = 0; i < edges_count; ++i) {
    std::string vertice;
    in >> vertice;
    graph.add_vertice(vertice);
  }
  graphs[graph_name] = graph;
}

void maslevtsov::merge_graphs(graphs_map_t& graphs, std::istream& in)
{
  std::string new_gr_name, gr1_name, gr2_name;
  in >> new_gr_name >> gr1_name >> gr2_name;
  if (graphs.find(new_gr_name) != graphs.end()) {
    throw std::invalid_argument("invalid graph name");
  }
  Graph new_gr(graphs.at(gr1_name), graphs.at(gr2_name));
  graphs[new_gr_name] = new_gr;
}

void maslevtsov::extract_from_graph(graphs_map_t& graphs, std::istream& in)
{
  std::string new_gr_name, gr_name;
  in >> new_gr_name >> gr_name;
  if (graphs.find(new_gr_name) != graphs.end()) {
    throw std::invalid_argument("invalid graph name");
  }
  size_t vertice_count = 0;
  in >> vertice_count;
  Vector< std::string > vertices;
  Graph gr = graphs.at(gr_name);
  for (size_t i = 0; i != vertice_count; ++i) {
    std::string vertice;
    in >> vertice;
    if (!gr.check_vertice_existence(vertice)) {
      throw std::invalid_argument("non-existing vertice given");
    }
    vertices.push_back(vertice);
  }
  Graph new_gr(graphs[gr_name], vertices);
  graphs[new_gr_name] = new_gr;
}
