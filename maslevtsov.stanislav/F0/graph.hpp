#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <hash_table/declaration.hpp>
#include <string>
#include <vector/declaration.hpp>

namespace maslevtsov {
  class Graph
  {
  public:
    using adjacency_list_t = maslevtsov::HashTable< unsigned, maslevtsov::Vector< unsigned > >;

    Graph() = default;
    Graph(const Graph& gr1, const Graph& gr2);
    Graph(const Graph& src, const maslevtsov::Vector< unsigned >& vertices);

    const adjacency_list_t& get_adj_list() const;

    void add_vertice(unsigned vertice);
    void add_edge(unsigned vertice1, unsigned vertice2);
    void delete_vertice(unsigned vertice);
    void delete_edge(unsigned vertice1, unsigned vertice2);

  private:
    adjacency_list_t adjacency_list_;

    friend std::istream& operator>>(std::istream& in, Graph& gr);
    friend std::ostream& operator<<(std::ostream& out, const Graph& gr);
  };

  std::istream& operator>>(std::istream& in, Graph& gr);
  std::ostream& operator<<(std::ostream& out, const Graph& gr);
}

#endif
