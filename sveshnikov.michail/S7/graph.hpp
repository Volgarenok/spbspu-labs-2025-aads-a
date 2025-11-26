#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <unordered_map>
#include <tree/tree.hpp>
#include <array.hpp>
#include "pair_hash.hpp"

namespace sveshnikov
{
  class Graph
  {
  public:
    Graph() = default;
    Graph(const Graph &graph1, const Graph &graph2);

    void add_vertex(const std::string &vert);
    void delete_vertex(const std::string &vert);
    void bind(const std::string &vert_out, const std::string &vert_in, unsigned int weight);
    void cut(const std::string &vert_out, const std::string &vert_in, unsigned int weight);

    Array< std::string > get_vertexes() const;
    AvlTree< std::string, Array< unsigned int > > get_outbounds(const std::string &vert) const;
    AvlTree< std::string, Array< unsigned int > > get_inbounds(const std::string &vert) const;
    bool check_vert_existance(const std::string &vertex) const;

  private:
    std::unordered_map< std::pair< std::string, std::string >, Array< unsigned int >, PairHash >
        graph_;
    AvlTree< std::string, char > vertexes_;
  };
}

#endif
