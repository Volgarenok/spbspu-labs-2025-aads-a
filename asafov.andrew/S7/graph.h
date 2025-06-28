#ifndef GRAPH_H
#define GRAPH_H
#include <string>
#include <vector>
#include <unordered_map.hpp>

namespace asafov
{
  class Graph
  {
  private:
    std::unordered_map< std::string, std::vector< unsigned > > vertex_map;
    std::unordered_map< std::string, std::unordered_map< std::string, std::vector< unsigned > > > edges;

    static void sort_vector(std::vector< unsigned >& vec);

  public:
    void add_vertex(const std::string& name);
    bool has_vertex(const std::string& name) const;
    std::vector< std::string > get_vertices() const;
    void add_edge(const std::string& from, const std::string& to, unsigned weight);
    bool remove_edge(const std::string& from, const std::string& to, unsigned weight);

    std::unordered_map< std::string, std::vector< unsigned > > get_outbound(const std::string& v) const;
    std::unordered_map< std::string, std::vector< unsigned > > get_inbound(const std::string& v) const;

    void merge_from(const Graph& g);
    Graph extract(const std::vector< std::string >& subset) const;
  };
}
#endif
