#ifndef GRAPH_MANAGER_H
#define GRAPH_MANAGER_H
#include "Graph.h"
#include <string>
#include <unordered_map>

namespace asafov
{
  class GraphManager
  {
  private:
    std::unordered_map< std::string, Graph > graphs;

  public:
    void read_from_file(const std::string& filename);
    void process_commands();
  };
}
#endif
