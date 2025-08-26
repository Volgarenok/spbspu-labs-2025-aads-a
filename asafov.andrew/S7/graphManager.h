#ifndef GRAPH_MANAGER_H
#define GRAPH_MANAGER_H
#include "graph.h"
#include <string>
#include <unordered_map.hpp>

namespace asafov
{
  class GraphManager
  {
  private:
    asafov::UnorderedMap< std::string, Graph > graphs;

  public:
    void read_from_file(const std::string& filename);
    void process_commands();
  };
}
#endif
