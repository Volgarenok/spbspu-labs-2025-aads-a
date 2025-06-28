#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S7_GRAPH_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S7_GRAPH_HPP

#include <map>
#include <set>
#include <string>
#include <unordered-map.hpp>

namespace kizhin {
  using Vertex = std::string;
  struct VertexPair
  {
    Vertex from{};
    Vertex to{};
  };
}

namespace std {
  template <>
  struct hash< kizhin::VertexPair >
  {
    size_t operator()(const kizhin::VertexPair&) const;
  };
}

namespace kizhin {
  using Weight = std::size_t;
  using WeightContiner = std::multiset< Weight >;

  bool operator==(const VertexPair&, const VertexPair&);
  bool operator<(const VertexPair&, const VertexPair&);

  using Graph = UnorderedMap< VertexPair, WeightContiner >;
  using VertexContainer = std::set< Vertex >;
  using SortedGraph = std::map< Graph::key_type, Graph::mapped_type >;

  VertexContainer getVertices(const Graph&);
  SortedGraph getOutbound(const Graph&, const Vertex&);
  SortedGraph getInbound(const Graph&, const Vertex&);
}

#endif

