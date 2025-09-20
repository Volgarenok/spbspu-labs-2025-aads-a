#ifndef BFS_COMMANDS_HPP
#define BFS_COMMANDS_HPP

#include <iostream>
#include "graph.hpp"

namespace maslevtsov {
  using graphs_t = maslevtsov::HashTable< std::string, Graph >;

  void traverse_breadth_first(const graphs_t& graphs, std::istream& in, std::ostream& out);
  void get_min_path(const graphs_t& graphs, std::istream& in, std::ostream& out);
  void get_graph_width(const graphs_t& graphs, std::istream& in, std::ostream& out);
  void get_graph_components(const graphs_t& graphs, std::istream& in, std::ostream& out);
}

#endif
