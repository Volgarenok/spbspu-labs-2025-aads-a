#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <cctype>
#include <cstdlib>

struct pair_hash
{
  std::size_t operator()(const std::pair< std::string, std::string >& p) const
  {
    return std::hash< std::string >()(p.first) ^ (std::hash< std::string >()(p.second) << 1);
  }
};

struct Graph
{
  std::unordered_map< std::pair< std::string, std::string >, std::vector< unsigned >, pair_hash > edges;
  std::vector< std::string > vertices;

  bool has_vertex(const std::string& v) const
  {
    for (std::size_t i = 0; i < vertices.size(); ++i)
    {
      if (vertices[i] == v) return true;
    }
    return false;
  }

  void add_vertex(const std::string& v)
  {
    if (!has_vertex(v))
    {
      vertices.push_back(v);
    }
  }

  void add_edge(const std::string& from, const std::string& to, unsigned weight)
  {
    add_vertex(from);
    add_vertex(to);
    edges[std::make_pair(from, to)].push_back(weight);
  }

  bool remove_edge(const std::string& from, const std::string& to, unsigned weight)
  {
    std::pair< std::string, std::string > key = std::make_pair(from, to);
    if (edges.count(key))
    {
      std::vector< unsigned >& weights = edges[key];
      for (std::size_t i = 0; i < weights.size(); ++i)
      {
        if (weights[i] == weight)
        {
          weights.erase(weights.begin() + i);
          if (weights.empty())
          {
            edges.erase(key);
          }
          return true;
        }
      }
    }
    return false;
  }

  void get_outbound(const std::string& vertex) const
  {
    if (!has_vertex(vertex))
    {
      std::cout << "<INVALID COMMAND>" << '\n';
      return;
    }
    std::map< std::string, std::vector< unsigned > > result;
    for (auto it = edges.begin(); it != edges.end(); ++it)
    {
      if (it->first.first == vertex)
      {
        result[it->first.second].insert(result[it->first.second].end(),
                                        it->second.begin(), it->second.end());
      }
    }
    for (std::map< std::string, std::vector< unsigned > >::iterator it = result.begin(); it != result.end(); ++it)
    {
      std::cout << it->first;
      sort_weights(it->second);
      for (std::size_t i = 0; i < it->second.size(); ++i)
        std::cout << " " << it->second[i];
      std::cout << '\n';
    }
  }

  void get_inbound(const std::string& vertex) const
  {
    if (!has_vertex(vertex))
    {
      std::cout << "<INVALID COMMAND>" << '\n';
      return;
    }
    std::map< std::string, std::vector< unsigned > > result;
    for (auto it = edges.begin(); it != edges.end(); ++it)
    {
      if (it->first.second == vertex)
      {
        result[it->first.first].insert(result[it->first.first].end(),
                                       it->second.begin(), it->second.end());
      }
    }
    for (std::map< std::string, std::vector< unsigned > >::iterator it = result.begin(); it != result.end(); ++it)
    {
      std::cout << it->first;
      sort_weights(it->second);
      for (std::size_t i = 0; i < it->second.size(); ++i)
        std::cout << " " << it->second[i];
      std::cout << '\n';
    }
  }

  static void sort_weights(std::vector< unsigned >& weights)
  {
    for (std::size_t i = 0; i < weights.size(); ++i)
    {
      for (std::size_t j = i + 1; j < weights.size(); ++j)
      {
        if (weights[j] < weights[i])
        {
          unsigned tmp = weights[i];
          weights[i] = weights[j];
          weights[j] = tmp;
        }
      }
    }
  }
};

std::map< std::string, Graph > graphs;

void trim(std::string& s)
{
  while (!s.empty() && std::isspace(s[s.size() - 1])) s.pop_back();
  std::size_t i = 0;
  while (i < s.size() && std::isspace(s[i])) ++i;
  s = s.substr(i);
}

std::vector< std::string > split(const std::string& s)
{
  std::vector< std::string > result;
  std::string word;
  for (std::size_t i = 0; i <= s.size(); ++i)
  {
    if (i == s.size() || s[i] == ' ')
    {
      if (!word.empty())
      {
        result.push_back(word);
        word.clear();
      }
    }
    else
    {
      word += s[i];
    }
  }
  return result;
}

void read_graphs_from_file(const std::string& filename)
{
  std::ifstream fin(filename.c_str());
  if (!fin)
  {
    std::cerr << "Cannot open file: " << filename << '\n';
    std::exit(1);
  }

  std::string line;
  while (std::getline(fin, line))
  {
    trim(line);
    if (line.empty()) continue;

    std::vector< std::string > parts = split(line);
    if (parts.size() != 2) continue;

    std::string graph_name = parts[0];
    int edge_count = std::atoi(parts[1].c_str());
    Graph g;

    for (int i = 0; i < edge_count && std::getline(fin, line);)
    {
      trim(line);
      if (line.empty()) continue;
      std::vector< std::string > edge_parts = split(line);
      if (edge_parts.size() != 3) continue;

      std::string from = edge_parts[0], to = edge_parts[1];
      unsigned weight = static_cast< unsigned >(std::atoi(edge_parts[2].c_str()));
      g.add_edge(from, to, weight);
      ++i;
    }
    graphs[graph_name] = g;
  }
}

void process_commands()
{
  std::string line;
  while (std::getline(std::cin, line))
  {
    trim(line);
    if (line.empty()) continue;
    std::vector< std::string > parts = split(line);
    if (parts.empty()) continue;

    std::string cmd = parts[0];

    if (cmd == "graphs")
    {
      for (std::map< std::string, Graph >::iterator it = graphs.begin(); it != graphs.end(); ++it)
        std::cout << it->first << '\n';
    }
    else if (cmd == "vertexes" && parts.size() == 2)
    {
      if (!graphs.count(parts[1]))
      {
        std::cout << "<INVALID COMMAND>" << '\n';
        continue;
      }
      std::vector< std::string > vs = graphs[parts[1]].vertices;
      for (std::size_t i = 0; i < vs.size(); ++i)
      {
        for (std::size_t j = i + 1; j < vs.size(); ++j)
        {
          if (vs[j] < vs[i])
          {
            std::string tmp = vs[i];
            vs[i] = vs[j];
            vs[j] = tmp;
          }
        }
      }
      for (std::size_t i = 0; i < vs.size(); ++i) std::cout << vs[i] << '\n';
    }
    else if (cmd == "outbound" && parts.size() == 3)
    {
      if (!graphs.count(parts[1]))
      {
        std::cout << "<INVALID COMMAND>" << '\n';
        continue;
      }
      graphs[parts[1]].get_outbound(parts[2]);
    }
    else if (cmd == "inbound" && parts.size() == 3)
    {
      if (!graphs.count(parts[1]))
      {
        std::cout << "<INVALID COMMAND>" << '\n';
        continue;
      }
      graphs[parts[1]].get_inbound(parts[2]);
    }
    else if (cmd == "bind" && parts.size() == 5)
    {
      if (!graphs.count(parts[1]))
      {
        std::cout << "<INVALID COMMAND>" << '\n';
        continue;
      }
      unsigned w = static_cast< unsigned >(std::atoi(parts[4].c_str()));
      graphs[parts[1]].add_edge(parts[2], parts[3], w);
    }
    else if (cmd == "cut" && parts.size() == 5)
    {
      if (!graphs.count(parts[1]) ||
        !graphs[parts[1]].has_vertex(parts[2]) ||
        !graphs[parts[1]].has_vertex(parts[3]) ||
        !graphs[parts[1]].remove_edge(parts[2], parts[3], static_cast< unsigned >(std::atoi(parts[4].c_str()))))
      {
        std::cout << "<INVALID COMMAND>" << '\n';
        continue;
      }
    }
    else if (cmd == "create" && parts.size() >= 2)
    {
      std::string name = parts[1];
      if (graphs.count(name))
      {
        std::cout << "<INVALID COMMAND>" << '\n';
        continue;
      }
      Graph g;
      for (std::size_t i = 2; i < parts.size(); ++i)
      {
        g.add_vertex(parts[i]);
      }
      graphs[name] = g;
    }
    else if (cmd == "merge" && parts.size() == 4)
    {
      std::string newg = parts[1], g1 = parts[2], g2 = parts[3];
      if (graphs.count(newg) || !graphs.count(g1) || !graphs.count(g2))
      {
        std::cout << "<INVALID COMMAND>" << '\n';
        continue;
      }
      Graph g;
      for (std::size_t i = 0; i < graphs[g1].vertices.size(); ++i)
        g.add_vertex(graphs[g1].vertices[i]);
      for (std::size_t i = 0; i < graphs[g2].vertices.size(); ++i)
        g.add_vertex(graphs[g2].vertices[i]);
      for (auto it = graphs[g1].edges.begin(); it != graphs[g1].edges.end(); ++it)
      {
        g.edges[it->first].insert(g.edges[it->first].end(),
                                  it->second.begin(), it->second.end());
      }
      for (auto it = graphs[g2].edges.begin(); it != graphs[g2].edges.end(); ++it)
      {
        g.edges[it->first].insert(g.edges[it->first].end(),
                                  it->second.begin(), it->second.end());
      }
      graphs[newg] = g;
    }
    else if (cmd == "extract" && parts.size() >= 4)
    {
      std::string newg = parts[1], oldg = parts[2];
      int k = std::atoi(parts[3].c_str());
      if (static_cast< int >(parts.size()) != 4 + k || graphs.count(newg) == 1 || !graphs.count(oldg))
      {
        std::cout << "<INVALID COMMAND>" << '\n';
        continue;
      }
      Graph g;
      for (int i = 0; i < k; ++i)
      {
        if (!graphs[oldg].has_vertex(parts[4 + i]))
        {
          std::cout << "<INVALID COMMAND>" << '\n';
          goto skip;
        }
        g.add_vertex(parts[4 + i]);
      }
      for (auto it = graphs[oldg].edges.begin(); it != graphs[oldg].edges.end(); ++it)
      {
        std::string from = it->first.first, to = it->first.second;
        if (g.has_vertex(from) && g.has_vertex(to))
        {
          g.edges[it->first] = it->second;
        }
      }
      graphs[newg] = g;
    skip:
      continue;
    }
    else
    {
      std::cout << "<INVALID COMMAND>" << '\n';
    }
  }
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: ./graph_tool <filename>" << '\n';
    return 1;
  }

  read_graphs_from_file(argv[1]);
  process_commands();

  return 0;
}
