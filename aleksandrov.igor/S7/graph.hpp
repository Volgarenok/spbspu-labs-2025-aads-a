#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include "hash-table.hpp"
#include <tree/tree.hpp>
#include <vector>

namespace aleksandrov
{
  struct Graph
  {
    using VertexName = std::string;
    using VertexPair = std::pair< VertexName, VertexName >;
    using Weights = std::vector< unsigned int >;

    struct VertexPairHash
    {
      template< class T1, class T2 >
      size_t operator()(const std::pair< T1, T2 >& p) const
      {
        auto h1 = std::hash< T1 >{}(p.first);
        auto h2 = std::hash< T2 >{}(p.second);
        return h1 ^ (h2 << 1);
      }
    };

    HashTable< VertexPair, Weights, VertexPairHash > edges;
    Tree< VertexName, bool > vertices;

    void bind(const VertexName& from, const VertexName& to, unsigned int w);
    void cut(const VertexName& from, const VertexName& to, unsigned int w);

    using Bounds = Tree< VertexName, std::vector< unsigned int > >;
    Bounds getOutBounds(const VertexName& from) const;
    Bounds getInBounds(const VertexName& to) const;
  };
}

#endif

