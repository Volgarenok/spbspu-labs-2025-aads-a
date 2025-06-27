#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cctype>

struct Edge
{
  std::string from;
  std::string to;
  unsigned weight;
};

class Graph
{
private:
  std::unordered_map< std::string, std::vector< Edge > > outbound_edges;
  std::unordered_map< std::string, std::vector< Edge > > inbound_edges;
  std::unordered_set< std::string > vertices;

public:
  void add_edge(const std::string& from, const std::string& to, unsigned weight)
  {
    Edge e{from, to, weight};
    outbound_edges[from].push_back(e);
    inbound_edges[to].push_back(e);
    vertices.insert(from);
    vertices.insert(to);
  }

  void remove_edge(const std::string& from, const std::string& to, unsigned weight)
  {
    bool found = false;

    // Remove from outbound
    auto& out_edges = outbound_edges[from];
    for (auto it = out_edges.begin(); it != out_edges.end();)
    {
      if (it->to == to && it->weight == weight)
      {
        it = out_edges.erase(it);
        found = true;
      }
      else
      {
        ++it;
      }
    }

    // Remove from inbound
    auto& in_edges = inbound_edges[to];
    for (auto it = in_edges.begin(); it != in_edges.end();)
    {
      if (it->from == from && it->weight == weight)
      {
        it = in_edges.erase(it);
      }
      else
      {
        ++it;
      }
    }

    // Clean up if no more edges
    if (out_edges.empty())
    {
      outbound_edges.erase(from);
    }
    if (in_edges.empty())
    {
      inbound_edges.erase(to);
    }

    // Remove vertices if they have no edges
    if (outbound_edges.count(from) == 0 && inbound_edges.count(from) == 0)
    {
      vertices.erase(from);
    }
    if (from != to && outbound_edges.count(to) == 0 && inbound_edges.count(to) == 0)
    {
      vertices.erase(to);
    }

    if (!found)
    {
      throw std::runtime_error("Edge not found");
    }
  }

  std::vector< std::string > get_vertices() const
  {
    std::vector< std::string > result;
    for (const auto& v: vertices)
    {
      result.push_back(v);
    }
    std::sort(result.begin(), result.end());
    return result;
  }

  std::vector< Edge > get_outbound_edges(const std::string& vertex) const
  {
    auto it = outbound_edges.find(vertex);
    if (it != outbound_edges.end())
    {
      return it->second;
    }
    return {};
  }

  std::vector< Edge > get_inbound_edges(const std::string& vertex) const
  {
    auto it = inbound_edges.find(vertex);
    if (it != inbound_edges.end())
    {
      return it->second;
    }
    return {};
  }

  void add_vertex(const std::string& vertex)
  {
    vertices.insert(vertex);
  }

  bool has_vertex(const std::string& vertex) const
  {
    return vertices.count(vertex) > 0;
  }

  static Graph merge(const Graph& g1, const Graph& g2)
  {
    Graph result;

    // Add all vertices
    for (const auto& v: g1.vertices)
    {
      result.add_vertex(v);
    }
    for (const auto& v: g2.vertices)
    {
      result.add_vertex(v);
    }

    // Add all edges from g1
    for (const auto& pair: g1.outbound_edges)
    {
      for (const auto& edge: pair.second)
      {
        result.add_edge(edge.from, edge.to, edge.weight);
      }
    }

    // Add all edges from g2
    for (const auto& pair: g2.outbound_edges)
    {
      for (const auto& edge: pair.second)
      {
        result.add_edge(edge.from, edge.to, edge.weight);
      }
    }

    return result;
  }

  static Graph extract(const Graph& g, const std::unordered_set< std::string >& vertices)
  {
    Graph result;

    // Add selected vertices
    for (const auto& v: vertices)
    {
      if (g.has_vertex(v))
      {
        result.add_vertex(v);
      }
      else
      {
        throw std::runtime_error("Vertex not found in original graph");
      }
    }

    // Add edges between selected vertices
    for (const auto& v: vertices)
    {
      auto it = g.outbound_edges.find(v);
      if (it == g.outbound_edges.end()) continue;

      for (const auto& edge: it->second)
      {
        if (vertices.count(edge.to))
        {
          result.add_edge(edge.from, edge.to, edge.weight);
        }
      }
    }

    return result;
  }
};

class GraphManager
{
private:
  std::unordered_map< std::string, Graph > graphs;

  std::vector< std::string > split(const std::string& s)
  {
    std::vector< std::string > tokens;
    size_t start = 0;
    size_t end = s.find(' ');

    while (end != std::string::npos)
    {
      std::string token = s.substr(start, end - start);
      if (!token.empty())
      {
        tokens.push_back(token);
      }
      start = end + 1;
      end = s.find(' ', start);
    }
    std::string last_token = s.substr(start);
    if (!last_token.empty())
    {
      tokens.push_back(last_token);
    }

    return tokens;
  }

public:
  void add_graph(const std::string& name, const Graph& graph)
  {
    graphs[name] = graph;
  }

  void process_command(const std::string& command_line)
  {
    std::vector< std::string > tokens = split(command_line);
    if (tokens.empty()) return;

    try
    {
      const std::string& command = tokens[0];

      if (command == "graphs")
      {
        cmd_graphs();
      }
      else if (command == "vertexes")
      {
        cmd_vertexes(tokens);
      }
      else if (command == "outbound")
      {
        cmd_outbound(tokens);
      }
      else if (command == "inbound")
      {
        cmd_inbound(tokens);
      }
      else if (command == "bind")
      {
        cmd_bind(tokens);
      }
      else if (command == "cut")
      {
        cmd_cut(tokens);
      }
      else if (command == "create")
      {
        cmd_create(tokens);
      }
      else if (command == "merge")
      {
        cmd_merge(tokens);
      }
      else if (command == "extract")
      {
        cmd_extract(tokens);
      }
      else
      {
        throw std::runtime_error("Unknown command");
      }
    }
    catch (const std::exception& e)
    {
      std::cout << "<INVALID COMMAND>" << '\n';
    }
  }

private:
  void cmd_graphs()
  {
    std::vector< std::string > names;
    for (const auto& pair: graphs)
    {
      names.push_back(pair.first);
    }
    std::sort(names.begin(), names.end());
    for (const auto& name: names)
    {
      std::cout << name << '\n';
    }
  }

  void cmd_vertexes(const std::vector< std::string >& tokens)
  {
    if (tokens.size() < 2) throw std::runtime_error("Missing graph name");

    const std::string& graph_name = tokens[1];
    if (graphs.count(graph_name) == 0)
    {
      throw std::runtime_error("Invalid graph name");
    }

    std::vector< std::string > vertices = graphs[graph_name].get_vertices();
    for (const auto& v: vertices)
    {
      std::cout << v << '\n';
    }
  }

  void cmd_outbound(const std::vector< std::string >& tokens)
  {
    if (tokens.size() < 3) throw std::runtime_error("Missing parameters");

    const std::string& graph_name = tokens[1];
    const std::string& vertex = tokens[2];

    if (graphs.count(graph_name) == 0)
    {
      throw std::runtime_error("Invalid graph name");
    }

    const Graph& g = graphs[graph_name];
    if (!g.has_vertex(vertex))
    {
      throw std::runtime_error("Vertex not found");
    }

    std::vector< Edge > edges = g.get_outbound_edges(vertex);
    std::unordered_map< std::string, std::vector< unsigned > > targets;
    for (const auto& e: edges)
    {
      targets[e.to].push_back(e.weight);
    }

    // Sort target vertices
    std::vector< std::string > sorted_targets;
    for (const auto& pair: targets)
    {
      sorted_targets.push_back(pair.first);
    }
    std::sort(sorted_targets.begin(), sorted_targets.end());

    // Output sorted results
    for (const auto& target: sorted_targets)
    {
      std::vector< unsigned > weights = targets[target];
      std::sort(weights.begin(), weights.end());

      std::cout << target;
      for (unsigned w: weights)
      {
        std::cout << " " << w;
      }
      std::cout << '\n';
    }
  }

  void cmd_inbound(const std::vector< std::string >& tokens)
  {
    if (tokens.size() < 3) throw std::runtime_error("Missing parameters");

    const std::string& graph_name = tokens[1];
    const std::string& vertex = tokens[2];

    if (graphs.count(graph_name) == 0)
    {
      throw std::runtime_error("Invalid graph name");
    }

    const Graph& g = graphs[graph_name];
    if (!g.has_vertex(vertex))
    {
      throw std::runtime_error("Vertex not found");
    }

    std::vector< Edge > edges = g.get_inbound_edges(vertex);
    std::unordered_map< std::string, std::vector< unsigned > > sources;
    for (const auto& e: edges)
    {
      sources[e.from].push_back(e.weight);
    }

    // Sort source vertices
    std::vector< std::string > sorted_sources;
    for (const auto& pair: sources)
    {
      sorted_sources.push_back(pair.first);
    }
    std::sort(sorted_sources.begin(), sorted_sources.end());

    // Output sorted results
    for (const auto& source: sorted_sources)
    {
      std::vector< unsigned > weights = sources[source];
      std::sort(weights.begin(), weights.end());

      std::cout << source;
      for (unsigned w: weights)
      {
        std::cout << " " << w;
      }
      std::cout << '\n';
    }
  }

  void cmd_bind(const std::vector< std::string >& tokens)
  {
    if (tokens.size() < 5) throw std::runtime_error("Missing parameters");

    const std::string& graph_name = tokens[1];
    const std::string& from = tokens[2];
    const std::string& to = tokens[3];
    unsigned weight;

    try
    {
      weight = std::stoul(tokens[4]);
    }
    catch (...)
    {
      throw std::runtime_error("Invalid weight");
    }

    if (graphs.count(graph_name) == 0)
    {
      throw std::runtime_error("Invalid graph name");
    }

    graphs[graph_name].add_edge(from, to, weight);
  }

  void cmd_cut(const std::vector< std::string >& tokens)
  {
    if (tokens.size() < 5) throw std::runtime_error("Missing parameters");

    const std::string& graph_name = tokens[1];
    const std::string& from = tokens[2];
    const std::string& to = tokens[3];
    unsigned weight;

    try
    {
      weight = std::stoul(tokens[4]);
    }
    catch (...)
    {
      throw std::runtime_error("Invalid weight");
    }

    if (graphs.count(graph_name) == 0)
    {
      throw std::runtime_error("Invalid graph name");
    }

    Graph& g = graphs[graph_name];
    if (!g.has_vertex(from) || !g.has_vertex(to))
    {
      throw std::runtime_error("Vertex not found");
    }

    g.remove_edge(from, to, weight);
  }

  void cmd_create(const std::vector< std::string >& tokens)
  {
    if (tokens.size() < 2) throw std::runtime_error("Missing graph name");

    const std::string& graph_name = tokens[1];
    if (graphs.count(graph_name) > 0)
    {
      throw std::runtime_error("Graph already exists");
    }

    Graph g;
    if (tokens.size() > 2)
    {
      try
      {
        int count = std::stoi(tokens[2]);
        if (count < 0 || tokens.size() < 3 + static_cast< size_t >(count))
        {
          throw std::runtime_error("Invalid vertex count");
        }
        for (int i = 0; i < count; ++i)
        {
          g.add_vertex(tokens[3 + i]);
        }
      }
      catch (...)
      {
        // If count is not a number, treat all remaining tokens as vertices
        for (size_t i = 2; i < tokens.size(); ++i)
        {
          g.add_vertex(tokens[i]);
        }
      }
    }

    graphs[graph_name] = g;
  }

  void cmd_merge(const std::vector< std::string >& tokens)
  {
    if (tokens.size() < 4) throw std::runtime_error("Missing parameters");

    const std::string& new_name = tokens[1];
    const std::string& graph1_name = tokens[2];
    const std::string& graph2_name = tokens[3];

    if (graphs.count(new_name) > 0 ||
      graphs.count(graph1_name) == 0 ||
      graphs.count(graph2_name) == 0)
    {
      throw std::runtime_error("Invalid parameters");
    }

    Graph merged = Graph::merge(graphs[graph1_name], graphs[graph2_name]);
    graphs[new_name] = merged;
  }

  void cmd_extract(const std::vector< std::string >& tokens)
  {
    if (tokens.size() < 5) throw std::runtime_error("Missing parameters");

    const std::string& new_name = tokens[1];
    const std::string& old_name = tokens[2];
    int count;

    try
    {
      count = std::stoi(tokens[3]);
    }
    catch (...)
    {
      throw std::runtime_error("Invalid count");
    }

    if (graphs.count(new_name) > 0 ||
      graphs.count(old_name) == 0 ||
      static_cast< int >(tokens.size()) < 4 + count)
    {
      throw std::runtime_error("Invalid parameters");
    }

    std::unordered_set< std::string > vertices;
    for (int i = 0; i < count; ++i)
    {
      vertices.insert(tokens[4 + i]);
    }

    Graph extracted = Graph::extract(graphs[old_name], vertices);
    graphs[new_name] = extracted;
  }
};

void load_graphs_from_file(const std::string& filename, GraphManager& manager)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    std::cerr << "Failed to open file: " << filename << '\n';
    return;
  }

  std::string line;
  while (std::getline(file, line))
  {
    // Skip empty lines
    if (line.empty()) continue;

    // Split line into tokens
    size_t space1 = line.find(' ');
    if (space1 == std::string::npos)
    {
      std::cerr << "Invalid graph header: " << line << '\n';
      continue;
    }

    std::string graph_name = line.substr(0, space1);
    unsigned edge_count;

    try
    {
      edge_count = std::stoul(line.substr(space1 + 1));
    }
    catch (...)
    {
      std::cerr << "Invalid edge count: " << line << '\n';
      continue;
    }

    Graph graph;
    for (unsigned i = 0; i < edge_count; ++i)
    {
      while (std::getline(file, line))
      {
        if (line.empty()) continue;
        break;
      }

      size_t space1 = line.find(' ');
      if (space1 == std::string::npos)
      {
        std::cerr << "Invalid edge format: " << line << '\n';
        --i; // Retry this edge
        continue;
      }

      size_t space2 = line.find(' ', space1 + 1);
      if (space2 == std::string::npos)
      {
        std::cerr << "Invalid edge format: " << line << '\n';
        --i;
        continue;
      }

      std::string from = line.substr(0, space1);
      std::string to = line.substr(space1 + 1, space2 - space1 - 1);
      unsigned weight;

      try
      {
        weight = std::stoul(line.substr(space2 + 1));
      }
      catch (...)
      {
        std::cerr << "Invalid weight: " << line << '\n';
        --i;
        continue;
      }

      graph.add_edge(from, to, weight);
    }

    manager.add_graph(graph_name, graph);
  }
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <graph-file>" << '\n';
    return 1;
  }

  GraphManager manager;
  load_graphs_from_file(argv[1], manager);

  std::string command_line;
  while (std::getline(std::cin, command_line))
  {
    if (command_line.empty()) continue;
    manager.process_command(command_line);
  }

  return 0;
}
