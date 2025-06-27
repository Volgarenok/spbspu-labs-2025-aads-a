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

  void Graph::add_edge(const std::string& from, const std::string& to, unsigned weight)
  {
    add_vertex(from);
    add_vertex(to);
    edges[from][to].push_back(weight);
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

  std::unordered_map< std::string, std::vector< unsigned > > Graph::get_outbound(const std::string& v) const
  {
    std::unordered_map< std::string, std::vector< unsigned > > result;
    if (edges.find(v) != edges.end())
    {
      const std::unordered_map< std::string, std::vector< unsigned > >& out = edges.at(v);
      for (std::unordered_map< std::string, std::vector< unsigned > >::const_iterator it = out.begin(); it != out.end();
           ++it)
      {
        std::vector< unsigned > sorted = it->second;
        for (size_t i = 0; i < sorted.size(); ++i)
        {
          for (size_t j = i + 1; j < sorted.size(); ++j)
          {
            if (sorted[j] < sorted[i])
            {
              unsigned tmp = sorted[i];
              sorted[i] = sorted[j];
              sorted[j] = tmp;
            }
          }
        }
        result[it->first] = sorted;
      }
    }
    return result;
  }

  std::unordered_map< std::string, std::vector< unsigned > > Graph::get_inbound(const std::string& v) const
  {
    std::unordered_map< std::string, std::vector< unsigned > > result;
    for (std::unordered_map< std::string, std::unordered_map< std::string, std::vector< unsigned > > >::const_iterator
         from_it = edges.begin(); from_it != edges.end(); ++from_it)
    {
      std::unordered_map< std::string, std::vector< unsigned > >::const_iterator it = from_it->second.find(v);
      if (it != from_it->second.end())
      {
        std::vector< unsigned > sorted = it->second;
        for (size_t i = 0; i < sorted.size(); ++i)
        {
          for (size_t j = i + 1; j < sorted.size(); ++j)
          {
            if (sorted[j] < sorted[i])
            {
              unsigned tmp = sorted[i];
              sorted[i] = sorted[j];
              sorted[j] = tmp;
            }
          }
        }
        result[from_it->first] = sorted;
      }
    }
    return result;
  }

  void Graph::merge_from(const Graph& g)
  {
    std::vector< std::string > v = g.get_vertices();
    for (size_t i = 0; i < v.size(); ++i)
    {
      add_vertex(v[i]);
    }

    for (std::unordered_map< std::string, std::unordered_map< std::string, std::vector< unsigned > > >::const_iterator
         it = g.edges.begin(); it != g.edges.end(); ++it)
    {
      for (std::unordered_map< std::string, std::vector< unsigned > >::const_iterator jt = it->second.begin(); jt != it
           ->second.end(); ++jt)
      {
        for (size_t k = 0; k < jt->second.size(); ++k)
        {
          add_edge(it->first, jt->first, jt->second[k]);
        }
      }
    }
  }

  Graph Graph::extract(const std::vector< std::string >& subset) const
  {
    Graph newg;
    for (size_t i = 0; i < subset.size(); ++i)
    {
      if (!has_vertex(subset[i]))
      {
        Graph invalid;
        return invalid;
      }
      newg.add_vertex(subset[i]);
    }

    for (size_t i = 0; i < subset.size(); ++i)
    {
      if (edges.find(subset[i]) != edges.end())
      {
        const std::unordered_map< std::string, std::vector< unsigned > >& outgoing = edges.at(subset[i]);
        for (std::unordered_map< std::string, std::vector< unsigned > >::const_iterator jt = outgoing.begin(); jt !=
             outgoing.end(); ++jt)
        {
          bool found = false;
          for (size_t k = 0; k < subset.size(); ++k)
          {
            if (jt->first == subset[k])
            {
              found = true;
              break;
            }
          }
          if (found)
          {
            for (size_t w = 0; w < jt->second.size(); ++w)
            {
              newg.add_edge(subset[i], jt->first, jt->second[w]);
            }
          }
        }
      }
    }

    return newg;
  }
}
