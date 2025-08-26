#ifndef GRAPH_HPP
#define GRAPH_HPP
#include "edge.hpp"
#include <hash_table/hash_table.hpp>
#include "simple_array.hpp"

namespace abramov
{
  struct Graph
  {
    using Iterator = typename HashTable< std::string, SimpleArray< Edge > >::Iter;
    using ConstIterator = typename HashTable< std::string, SimpleArray< Edge > >::cIter;

    Graph(const std::string &n);
    Graph(const Graph &other);
    Graph &operator=(const Graph &other);
    void addVertex(const std::string &vert);
    void addEdge(const std::string &from, const std::string &to, size_t w);
    bool cutEdge(const std::string &from, const std::string &to, size_t w);
    const std::string &getName() const noexcept;
    size_t getEdgeCount() const noexcept;
    size_t getVertexCount() const noexcept;
    const SimpleArray< Edge > *getEdges(const std::string &vert) const;
    bool hasVertex(const std::string &vert) const;
    Iterator begin();
    Iterator end();
    ConstIterator cbegin() const;
    ConstIterator cend() const;
  private:
    HashTable< std::string, SimpleArray< Edge > > adj_list;
    std::string name;
    size_t edge_count;

    void swap(Graph &other) noexcept;
  };
}
#endif
