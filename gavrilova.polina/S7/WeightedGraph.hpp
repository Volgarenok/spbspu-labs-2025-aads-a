#ifndef WEIGHTED_GRAPH_HPP
#define WEIGHTED_GRAPH_HPP

#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include "HashTable.hpp"

namespace gavrilova {

  template < typename Vertex, typename Weight = unsigned >
  class WeightedGraph {
  public:
    using vertex_type = Vertex;
    using weight_type = Weight;
    using edge_type = std::pair< Vertex, Weight >;
    using adjacency_list = std::vector< edge_type >;

    void add_vertex(const Vertex& v);
    void add_edge(const Vertex& from, const Vertex& to, Weight weight);
    bool remove_edge(const Vertex& from, const Vertex& to, Weight weight);

    bool has_vertex(const Vertex& v) const;
    bool has_edge(const Vertex& from, const Vertex& to, Weight weight) const;

    std::vector< Vertex > get_vertices() const;
    adjacency_list get_outbound_edges(const Vertex& v) const;
    adjacency_list get_inbound_edges(const Vertex& v) const;
    size_t vertex_count() const;
    size_t edge_count() const;

  private:
    HashTable< Vertex, adjacency_list > outbound_edges_;
    HashTable< Vertex, adjacency_list > inbound_edges_;
    size_t edges_count_ = 0;

    bool remove_edge_from_list(adjacency_list& edges, const Vertex& target, Weight weight);
    void sort_edges(adjacency_list& edges) const;
    void sort_vertices(std::vector< Vertex >& vertices) const;
  };

  template < typename Vertex, typename Weight >
  WeightedGraph< Vertex, Weight > merge_graphs(
      const WeightedGraph< Vertex, Weight >& g1,
      const WeightedGraph< Vertex, Weight >& g2);

  template < typename Vertex, typename Weight >
  WeightedGraph< Vertex, Weight > extract_graph(
      const WeightedGraph< Vertex, Weight >& g,
      const std::vector< Vertex >& vertices);

  template < typename Vertex, typename Weight >
  void WeightedGraph< Vertex, Weight >::add_vertex(const Vertex& v)
  {
    if (outbound_edges_.find(v) == outbound_edges_.end()) {
      outbound_edges_.insert({v, adjacency_list()});
      inbound_edges_.insert({v, adjacency_list()});
    }
  }

  template < typename Vertex, typename Weight >
  void WeightedGraph< Vertex, Weight >::add_edge(const Vertex& from, const Vertex& to, Weight weight)
  {
    add_vertex(from);
    add_vertex(to);
    outbound_edges_[from].emplace_back(to, weight);
    inbound_edges_[to].emplace_back(from, weight);
    edges_count_++;
  }

  template < typename Vertex, typename Weight >
  bool WeightedGraph< Vertex, Weight >::remove_edge(const Vertex& from, const Vertex& to, Weight weight)
  {
    if (remove_edge_from_list(outbound_edges_[from], to, weight)) {
      remove_edge_from_list(inbound_edges_[to], from, weight);
      edges_count_--;
      return true;
    }
    return false;
  }

  template < typename Vertex, typename Weight >
  bool WeightedGraph< Vertex, Weight >::has_vertex(const Vertex& v) const
  {
    return outbound_edges_.find(v) != outbound_edges_.end();
  }

  template < typename Vertex, typename Weight >
  bool WeightedGraph< Vertex, Weight >::has_edge(const Vertex& from, const Vertex& to, Weight weight) const
  {
    if (!has_vertex(from)) return false;

    const auto& edges = outbound_edges_.at(from);
    for (const auto& edge: edges) {
      if (edge.first == to && edge.second == weight) {
        return true;
      }
    }
    return false;
  }

  template < typename Vertex, typename Weight >
  std::vector< Vertex > WeightedGraph< Vertex, Weight >::get_vertices() const
  {
    std::vector< Vertex > vertices;

    for (const auto& edge: outbound_edges_) {
      const auto& vertex = edge.first;
      vertices.push_back(vertex);
    }

    sort_vertices(vertices);
    return vertices;
  }

  template < typename Vertex, typename Weight >
  typename WeightedGraph< Vertex, Weight >::adjacency_list
  WeightedGraph< Vertex, Weight >::get_outbound_edges(const Vertex& v) const
  {
    if (!has_vertex(v)) throw std::out_of_range("Vertex not found");
    return outbound_edges_.at(v);
  }

  template < typename Vertex, typename Weight >
  typename WeightedGraph< Vertex, Weight >::adjacency_list
  WeightedGraph< Vertex, Weight >::get_inbound_edges(const Vertex& v) const
  {
    if (!has_vertex(v)) throw std::out_of_range("Vertex not found");
    return inbound_edges_.at(v);
  }

  template < typename Vertex, typename Weight >
  size_t WeightedGraph< Vertex, Weight >::vertex_count() const
  {
    return outbound_edges_.size();
  }

  template < typename Vertex, typename Weight >
  size_t WeightedGraph< Vertex, Weight >::edge_count() const
  {
    return edges_count_;
  }

  template < typename Vertex, typename Weight >
  bool WeightedGraph< Vertex, Weight >::remove_edge_from_list(
      adjacency_list& edges, const Vertex& target, Weight weight)
  {
    for (size_t i = 0; i < edges.size(); ++i) {
      if (edges[i].first == target && edges[i].second == weight) {
        edges[i] = edges.back();
        edges.pop_back();
        return true;
      }
    }
    return false;
  }

  template < typename Vertex, typename Weight >
  void WeightedGraph< Vertex, Weight >::sort_edges(adjacency_list& edges) const
  {
    for (size_t i = 0; i < edges.size(); ++i) {
      for (size_t j = i + 1; j < edges.size(); ++j) {
        if (edges[j].first < edges[i].first ||
            (edges[j].first == edges[i].first && edges[j].second < edges[i].second)) {
          std::swap(edges[i], edges[j]);
        }
      }
    }
  }

  template < typename Vertex, typename Weight >
  void WeightedGraph< Vertex, Weight >::sort_vertices(std::vector< Vertex >& vertices) const
  {
    for (size_t i = 0; i < vertices.size(); ++i) {
      for (size_t j = i + 1; j < vertices.size(); ++j) {
        if (vertices[j] < vertices[i]) {
          std::swap(vertices[i], vertices[j]);
        }
      }
    }
  }

  template < typename Vertex, typename Weight >
  WeightedGraph< Vertex, Weight > merge_graphs(
      const WeightedGraph< Vertex, Weight >& g1,
      const WeightedGraph< Vertex, Weight >& g2)
  {
    WeightedGraph< Vertex, Weight > result = g1;

    for (const auto& vertex: g2.get_vertices()) {
      result.add_vertex(vertex);
    }

    for (const auto& vertex: g2.get_vertices()) {
      const auto& edges = g2.get_outbound_edges(vertex);
      for (const auto& edge: edges) {
        const auto& to = edge.first;
        const auto& weight = edge.second;
        result.add_edge(vertex, to, weight);
      }
    }

    return result;
  }

  template < typename Vertex, typename Weight >
  WeightedGraph< Vertex, Weight > extract_graph(
      const WeightedGraph< Vertex, Weight >& g,
      const std::vector< Vertex >& vertices)
  {
    WeightedGraph< Vertex, Weight > result;

    for (const auto& vertex: vertices) {
      if (g.has_vertex(vertex)) {
        result.add_vertex(vertex);
      }
    }

    for (const auto& from: vertices) {
      if (!g.has_vertex(from)) continue;

      const auto& outbound_edges = g.get_outbound_edges(from);

      for (const auto& edge: outbound_edges) {
        const auto& to = edge.first;
        const auto& weight = edge.second;

        bool to_included = false;
        for (const auto& v: vertices) {
          if (v == to) {
            to_included = true;
            break;
          }
        }

        if (to_included) {
          result.add_edge(from, to, weight);
        }
      }
    }

    return result;
  }

}

#endif
