#ifndef GRAPH_COMMANDS_HPP
#define GRAPH_COMMANDS_HPP

#include <iostream>
#include "graph.hpp"

namespace maslevtsov {
  using graphs_t = maslevtsov::HashTable< std::string, Graph >;

  bool check_graphs_format(std::istream& in);
  void print_help_manual(std::ostream& out);

  void save_graphs(const graphs_t& graphs, std::istream& in);
  void open_graphs(graphs_t& graphs, std::istream& in, std::ostream& out);

  void add_graph(graphs_t& graphs, std::istream& in);
  void add_vertice(graphs_t& graphs, std::istream& in);
  void add_edge(graphs_t& graphs, std::istream& in);
  void print_graph(const graphs_t& graphs, std::istream& in, std::ostream& out);
  void copy_graph(graphs_t& graphs, std::istream& in);
  void delete_graph(graphs_t& graphs, std::istream& in);
  void delete_vertice(graphs_t& graphs, std::istream& in);
  void delete_edge(graphs_t& graphs, std::istream& in);
  void union_graphs(graphs_t& graphs, std::istream& in);
  void create_subgraph(graphs_t& graphs, std::istream& in);
}

#endif
