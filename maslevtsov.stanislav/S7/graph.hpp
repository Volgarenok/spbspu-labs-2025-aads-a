#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <hash_table/declaration.hpp>
#include <vector/declaration.hpp>
#include <tree/declaration.hpp>
#include "pair_hash.hpp"

namespace maslevtsov {
  class Graph
  {
  public:
    using vertices_pair_t = std::pair< std::string, std::string >;
    using weights_t = Vector< unsigned >;
    using edges_set_t = maslevtsov::HashTable< vertices_pair_t, weights_t, PairStringStdHash, PairStringSipHash >;

    Graph() = default;
    Graph(const Graph& src1, const Graph& src2);
    Graph(const Graph& src, Vector< std::string >& vertices);

    Tree< std::string, int > get_vertices() const;
    Tree< std::string, weights_t > get_outbound(const std::string& vertice) const;
    Tree< std::string, weights_t > get_inbound(const std::string& vertice) const;
    void bind(const std::string& vertice1, const std::string& vertice2, unsigned weight);
    bool check_vertice_existence(const std::string& vertice);
    void add_vertice(const std::string& vertice);
    void cut(const std::string& vertice1, const std::string& vertice2, unsigned weight);

  private:
    edges_set_t edges_set_;
  };
}

#endif
