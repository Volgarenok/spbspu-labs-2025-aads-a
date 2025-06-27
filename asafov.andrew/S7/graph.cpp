#include "Graph.h"
#include <algorithm>

namespace asafov
{
  void Graph::add_vertex(const std::string& name)
  {
    vertex_map[name];
  }

  bool Graph::has_vertex(const std::string& name) const
  {
    return vertex_map.find(name) != vertex_map.end();
  }

  std::vector< std::string > Graph::get_vertices() const
  {
    std::vector< std::string > result;
    for (const auto& p: vertex_map)
      result.push_back(p.first);
    std::sort(result.begin(), result.end());
    return result;
  }

  void Graph::add_edge(const std::string& from, const std::string& to, unsigned weight)
  {
    add_vertex(from);
    add_vertex(to);
    edges[from][to].push_back(weight);
  }

  bool Graph::remove_edge(const std::string& from, const std::string& to, unsigned weight)
  {
    auto it = edges.find(from);
    if (it == edges.end()) return false;

    auto jt = it->second.find(to);
    if (jt == it->second.end()) return false;

    auto& weights = jt->second;
    auto wt_it = std::find(weights.begin(), weights.end(), weight);
    if (wt_it == weights.end()) return false;

    weights.erase(wt_it);
    if (weights.empty())
    {
      it->second.erase(to);
      if (it->second.empty())
      {
        edges.erase(from);
      }
    }

    return true;
  }

  void Graph::sort_vector(std::vector< unsigned >& vec)
  {
    std::sort(vec.begin(), vec.end());
  }

  std::unordered_map< std::string, std::vector< unsigned > > Graph::get_outbound(const std::string& v) const
  {
    std::unordered_map< std::string, std::vector< unsigned > > result;
    auto it = edges.find(v);
    if (it != edges.end())
    {
      for (const auto& pair: it->second)
      {
        auto sorted = pair.second;
        sort_vector(sorted);
        result[pair.first] = sorted;
      }
    }
    return result;
  }

  std::unordered_map< std::string, std::vector< unsigned > > Graph::get_inbound(const std::string& v) const
  {
    std::unordered_map< std::string, std::vector< unsigned > > result;
    for (const auto& [from, inner_map]: edges)
    {
      auto jt = inner_map.find(v);
      if (jt != inner_map.end())
      {
        auto sorted = jt->second;
        sort_vector(sorted);
        result[from] = sorted;
      }
    }
    return result;
  }

  void Graph::merge_from(const Graph& g)
  {
    for (const auto& v: g.get_vertices())
    {
      add_vertex(v);
    }

    for (const auto& [from, targets]: g.edges)
    {
      for (const auto& [to, weights]: targets)
      {
        for (auto w: weights)
          add_edge(from, to, w);
      }
    }
  }

  Graph Graph::extract(const std::vector< std::string >& subset) const
  {
    Graph newg;
    for (const auto& v: subset)
    {
      if (!has_vertex(v))
        return Graph();
      newg.add_vertex(v);
    }

    for (const auto& from: subset)
    {
      auto it = edges.find(from);
      if (it != edges.end())
      {
        for (const auto& [to, weights]: it->second)
        {
          if (std::find(subset.begin(), subset.end(), to) != subset.end())
          {
            for (auto w: weights)
              newg.add_edge(from, to, w);
          }
        }
      }
    }
    return newg;
  }
}
