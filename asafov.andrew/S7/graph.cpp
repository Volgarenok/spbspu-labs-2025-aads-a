#include "graph.h"

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
    for (std::unordered_map< std::string, std::vector< unsigned > >::const_iterator it = vertex_map.begin(); it !=
         vertex_map.end(); ++it)
    {
      result.push_back(it->first);
    }
    for (size_t i = 0; i < result.size(); ++i)
    {
      for (size_t j = i + 1; j < result.size(); ++j)
      {
        if (result[j] < result[i])
        {
          std::string tmp = result[i];
          result[i] = result[j];
          result[j] = tmp;
        }
      }
    }
    return result;
  }

  bool Graph::remove_edge(const std::string& from, const std::string& to, unsigned weight)
  {
    if (edges.find(from) == edges.end()) return false;
    if (edges[from].find(to) == edges[from].end()) return false;
    std::vector< unsigned >& weights = edges[from][to];
    for (std::vector< unsigned >::iterator it = weights.begin(); it != weights.end(); ++it)
    {
      if (*it == weight)
      {
        weights.erase(it);
        if (weights.empty())
        {
          edges[from].erase(to);
          if (edges[from].empty())
          {
            edges.erase(from);
          }
        }
        return true;
      }
    }
    return false;
  }

  void Graph::add_edge(const std::string& from, const std::string& to, unsigned weight)
  {
    edges[from][to].push_back(weight);
  }

  std::unordered_map< std::string, std::vector< unsigned > > Graph::get_outbound(const std::string& vertex) const
  {
    std::unordered_map< std::string, std::vector< unsigned > > result;
    std::unordered_map< std::string, std::unordered_map< std::string, std::vector< unsigned > > >::const_iterator it =
      edges.find(vertex);
    if (it != edges.end())
    {
      result = it->second;
    }
    return result;
  }

  std::unordered_map< std::string, std::vector< unsigned > > Graph::get_inbound(const std::string& vertex) const
  {
    std::unordered_map< std::string, std::vector< unsigned > > result;
    for (std::unordered_map< std::string, std::unordered_map< std::string, std::vector< unsigned > > >::const_iterator
         it = edges.begin(); it != edges.end(); ++it)
    {
      const std::string& from = it->first;
      const std::unordered_map< std::string, std::vector< unsigned > >& targets = it->second;

      std::unordered_map< std::string, std::vector< unsigned > >::const_iterator jt = targets.find(vertex);
      if (jt != targets.end())
      {
        result[from] = jt->second;
      }
    }
    return result;
  }

  void Graph::merge_from(const Graph& g)
  {
    for (std::unordered_map< std::string, std::unordered_map< std::string, std::vector< unsigned > > >::const_iterator
         it = g.edges.begin(); it != g.edges.end(); ++it)
    {
      const std::string& from = it->first;
      const std::unordered_map< std::string, std::vector< unsigned > >& targets = it->second;

      for (std::unordered_map< std::string, std::vector< unsigned > >::const_iterator jt = targets.begin(); jt !=
           targets.end(); ++jt)
      {
        const std::string& to = jt->first;
        const std::vector< unsigned >& weights = jt->second;
        for (size_t i = 0; i < weights.size(); ++i)
        {
          add_edge(from, to, weights[i]);
        }
      }
    }
  }

  Graph Graph::extract(const std::vector< std::string >& nodes) const
  {
    Graph g;

    for (std::vector< std::string >::const_iterator ni = nodes.begin(); ni != nodes.end(); ++ni)
    {
      const std::string& from = *ni;

      std::unordered_map< std::string, std::unordered_map< std::string, std::vector< unsigned > > >::const_iterator it =
        edges.find(from);
      if (it == edges.end()) continue;

      const std::unordered_map< std::string, std::vector< unsigned > >& targets = it->second;

      for (std::unordered_map< std::string, std::vector< unsigned > >::const_iterator jt = targets.begin(); jt !=
           targets.end(); ++jt)
      {
        const std::string& to = jt->first;
        const std::vector< unsigned >& weights = jt->second;

        bool to_in_nodes = false;
        for (std::vector< std::string >::const_iterator check = nodes.begin(); check != nodes.end(); ++check)
        {
          if (*check == to)
          {
            to_in_nodes = true;
            break;
          }
        }

        if (to_in_nodes)
        {
          for (size_t i = 0; i < weights.size(); ++i)
          {
            g.add_edge(from, to, weights[i]);
          }
        }
      }
    }

    return g;
  }
}
