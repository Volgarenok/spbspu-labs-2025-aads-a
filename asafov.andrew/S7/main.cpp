#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <cstdlib>

class Graph
{
private:
  std::unordered_map< std::string, std::vector< unsigned > > vertex_map;
  std::unordered_map< std::string, std::unordered_map< std::string, std::vector< unsigned > > > edges;
public:
  void add_vertex(const std::string& name)
  {
    vertex_map[name];
  }

  bool has_vertex(const std::string& name) const
  {
    return vertex_map.find(name) != vertex_map.end();
  }

  std::vector< std::string > get_vertices() const
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

  void add_edge(const std::string& from, const std::string& to, unsigned weight)
  {
    add_vertex(from);
    add_vertex(to);
    edges[from][to].push_back(weight);
  }

  bool remove_edge(const std::string& from, const std::string& to, unsigned weight)
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

  std::map< std::string, std::vector< unsigned > > get_outbound(const std::string& v) const
  {
    std::map< std::string, std::vector< unsigned > > result;
    if (edges.find(v) != edges.end())
    {
      for (std::unordered_map< std::string, std::vector< unsigned > >::const_iterator it = edges.at(v).begin(); it !=
           edges.at(v).end(); ++it)
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

  std::map< std::string, std::vector< unsigned > > get_inbound(const std::string& v) const
  {
    std::map< std::string, std::vector< unsigned > > result;
    for (std::unordered_map< std::string, std::unordered_map< std::string, std::vector< unsigned > > >::const_iterator
         from_it = edges.begin(); from_it != edges.end(); ++from_it)
    {
      if (from_it->second.find(v) != from_it->second.end())
      {
        std::vector< unsigned > sorted = from_it->second.at(v);
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

  void merge_from(const Graph& g)
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

  Graph extract(const std::vector< std::string >& subset) const
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
        for (std::unordered_map< std::string, std::vector< unsigned > >::const_iterator jt = edges.at(subset[i]).begin()
             ; jt != edges.at(subset[i]).end(); ++jt)
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
};

std::map< std::string, Graph > graphs;

void read_graphs_from_file(const std::string& filename)
{
  std::ifstream in(filename.c_str());
  std::string line;
  while (std::getline(in, line))
  {
    if (line.empty()) continue;
    std::string name;
    int count = 0;
    size_t pos = line.find(' ');
    if (pos != std::string::npos)
    {
      name = line.substr(0, pos);
      count = std::atoi(line.substr(pos + 1).c_str());
    }
    else
    {
      continue;
    }
    Graph g;
    for (int i = 0; i < count; ++i)
    {
      std::getline(in, line);
      if (line.empty())
      {
        --i;
        continue;
      }
      size_t p1 = line.find(' ');
      size_t p2 = line.find(' ', p1 + 1);
      std::string from = line.substr(0, p1);
      std::string to = line.substr(p1 + 1, p2 - p1 - 1);
      unsigned weight = static_cast< unsigned >(std::atoi(line.substr(p2 + 1).c_str()));
      g.add_edge(from, to, weight);
    }
    graphs[name] = g;
  }
}

void process_commands()
{
  std::string line;
  while (std::getline(std::cin, line))
  {
    if (line.empty()) continue;
    std::vector< std::string > parts;
    size_t start = 0;
    while (true)
    {
      size_t space = line.find(' ', start);
      if (space == std::string::npos)
      {
        parts.push_back(line.substr(start));
        break;
      }
      parts.push_back(line.substr(start, space - start));
      start = space + 1;
    }

    if (parts[0] == "graphs")
    {
      std::vector< std::string > names;
      for (std::map< std::string, Graph >::iterator it = graphs.begin(); it != graphs.end(); ++it)
        names.push_back(it->first);
      if (names.empty())
      {
        std::cout << '\n';
        continue;
      }
      for (size_t i = 0; i < names.size(); ++i)
        std::cout << names[i] << '\n';
    }
    else if (parts[0] == "vertexes" && parts.size() == 2)
    {
      if (graphs.find(parts[1]) == graphs.end())
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      std::vector< std::string > v = graphs[parts[1]].get_vertices();
      if (v.empty())
      {
        std::cout << '\n';
      }
      else
      {
        for (size_t i = 0; i < v.size(); ++i)
          std::cout << v[i] << '\n';
      }
    }
    else if (parts[0] == "outbound" && parts.size() == 3)
    {
      if (graphs.find(parts[1]) == graphs.end() || !graphs[parts[1]].has_vertex(parts[2]))
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      std::map< std::string, std::vector< unsigned > > m = graphs[parts[1]].get_outbound(parts[2]);
      for (std::map< std::string, std::vector< unsigned > >::iterator it = m.begin(); it != m.end(); ++it)
      {
        std::cout << it->first;
        for (size_t i = 0; i < it->second.size(); ++i)
          std::cout << " " << it->second[i];
        std::cout << '\n';
      }
    }
    else if (parts[0] == "inbound" && parts.size() == 3)
    {
      if (graphs.find(parts[1]) == graphs.end() || !graphs[parts[1]].has_vertex(parts[2]))
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      std::map< std::string, std::vector< unsigned > > m = graphs[parts[1]].get_inbound(parts[2]);
      for (std::map< std::string, std::vector< unsigned > >::iterator it = m.begin(); it != m.end(); ++it)
      {
        std::cout << it->first;
        for (size_t i = 0; i < it->second.size(); ++i)
          std::cout << " " << it->second[i];
        std::cout << '\n';
      }
    }
    else if (parts[0] == "bind" && parts.size() == 5)
    {
      if (graphs.find(parts[1]) == graphs.end())
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      unsigned w = static_cast< unsigned >(std::atoi(parts[4].c_str()));
      graphs[parts[1]].add_edge(parts[2], parts[3], w);
    }
    else if (parts[0] == "cut" && parts.size() == 5)
    {
      if (graphs.find(parts[1]) == graphs.end() || !graphs[parts[1]].has_vertex(parts[2]) || !graphs[parts[1]].
        has_vertex(parts[3]) || !graphs[parts[1]].remove_edge(parts[2], parts[3],
                                                              static_cast< unsigned >(std::atoi(parts[4].c_str()))))
      {
        std::cout << "<INVALID COMMAND>\n";
      }
    }
    else if (parts[0] == "create")
    {
      if (parts.size() == 2)
      {
        if (graphs.find(parts[1]) != graphs.end())
        {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }
        Graph g;
        graphs[parts[1]] = g;
      }
      else if (parts.size() >= 3)
      {
        std::string name = parts[1];
        int k = std::atoi(parts[2].c_str());
        if (static_cast< int >(parts.size()) != 3 + k || graphs.find(name) != graphs.end())
        {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }
        Graph g;
        for (int i = 0; i < k; ++i)
        {
          g.add_vertex(parts[3 + i]);
        }
        graphs[name] = g;
      }
      else
      {
        std::cout << "<INVALID COMMAND>\n";
      }
    }
    else if (parts[0] == "merge" && parts.size() == 4)
    {
      std::string newg = parts[1];
      std::string g1 = parts[2];
      std::string g2 = parts[3];
      if (graphs.find(newg) != graphs.end() || graphs.find(g1) == graphs.end() || graphs.find(g2) == graphs.end())
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      Graph g;
      g.merge_from(graphs[g1]);
      g.merge_from(graphs[g2]);
      graphs[newg] = g;
    }
    else if (parts[0] == "extract")
    {
      if (parts.size() < 4)
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      std::string newg = parts[1];
      std::string oldg = parts[2];
      int k = std::atoi(parts[3].c_str());
      if (static_cast< int >(parts.size()) != 4 + k || graphs.find(newg) != graphs.end() || graphs.find(oldg) == graphs.
        end())
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      std::vector< std::string > subset;
      for (int i = 0; i < k; ++i)
        subset.push_back(parts[4 + i]);
      Graph g = graphs[oldg].extract(subset);
      if (g.get_vertices().empty() && !subset.empty())
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      graphs[newg] = g;
    }
    else
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}

int main(int argc, char* argv[])
{
  if (argc == 2)
  {
    read_graphs_from_file(argv[1]);
  }
  else
  {
    std::cerr << "args err\n";
    return 1;
  }
  process_commands();
  return 0;
}
