#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <hash_table/declaration.hpp>
#include "graph.hpp"

namespace maslevtsov {
  using graphs_map_t = maslevtsov::HashTable< std::string, Graph >;

  void print_graphs(const graphs_map_t& graphs, std::ostream& out);
  void print_vertices(const graphs_map_t& graphs, std::istream& in, std::ostream& out);
  void print_outbound(const graphs_map_t& graphs, std::istream& in, std::ostream& out);
  void print_inbound(const graphs_map_t& graphs, std::istream& in, std::ostream& out);
  void bind_vertices(graphs_map_t& graphs, std::istream& in);
  void cut_vertices(graphs_map_t& graphs, std::istream& in);
  void create_graph(graphs_map_t& graphs, std::istream& in);
  void merge_graphs(graphs_map_t& graphs, std::istream& in);
  void extract_from_graph(graphs_map_t& graphs, std::istream& in);
}

#endif
