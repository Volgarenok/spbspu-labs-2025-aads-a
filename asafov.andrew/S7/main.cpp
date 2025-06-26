#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>

namespace asafov
{
  using Vertex = std::string;
  using Weight = unsigned int;
  using Edge = std::pair< Vertex, Vertex >;
  using Weights = std::vector< Weight >;

  struct Graph
  {
    std::vector< Vertex > vertices;
    std::unordered_map< Edge, Weights > edges;

    bool hasVertex(const Vertex& v) const
    {
      for (const auto& vertex: vertices)
      {
        if (vertex == v) return true;
      }
      return false;
    }
  };

  class GraphManager
  {
  private:
    std::unordered_map< std::string, Graph > graphs;

    void addVertex(const std::string& graph_name, const Vertex& vertex)
    {
      if (!graphs[graph_name].hasVertex(vertex))
      {
        graphs[graph_name].vertices.push_back(vertex);
      }
    }

    void addEdge(const std::string& graph_name, const Vertex& from, const Vertex& to, Weight weight)
    {
      auto& edges = graphs[graph_name].edges;
      bool found = false;
      for (auto& w: edges[{from, to}])
      {
        if (w == weight)
        {
          found = true;
          break;
        }
      }
      if (!found)
      {
        edges[{from, to}].push_back(weight);
      }
      addVertex(graph_name, from);
      addVertex(graph_name, to);
    }

    bool graphExists(const std::string& name) const
    {
      return graphs.find(name) != graphs.end();
    }

    bool vertexExists(const std::string& graph_name, const Vertex& vertex) const
    {
      const auto it = graphs.find(graph_name);
      if (it == graphs.end()) return false;
      return it->second.hasVertex(vertex);
    }

    void sortWeights(Weights& weights) const
    {
      for (size_t i = 0; i < weights.size(); ++i)
      {
        for (size_t j = i + 1; j < weights.size(); ++j)
        {
          if (weights[i] > weights[j])
          {
            std::swap(weights[i], weights[j]);
          }
        }
      }
    }

    void sortVertices(std::vector< Vertex >& vertices) const
    {
      for (size_t i = 0; i < vertices.size(); ++i)
      {
        for (size_t j = i + 1; j < vertices.size(); ++j)
        {
          if (vertices[i] > vertices[j])
          {
            std::swap(vertices[i], vertices[j]);
          }
        }
      }
    }

  public:
    void loadGraphsFromFile(const std::string& filename)
    {
      std::ifstream file(filename);
      if (!file.is_open())
      {
        return;
      }

      std::string line;
      while (std::getline(file, line))
      {
        if (line.empty()) continue;

        size_t pos = 0;
        std::string graph_name;
        while (pos < line.size() && line[pos] != ' ')
        {
          graph_name += line[pos++];
        }
        if (pos >= line.size()) continue;
        pos++;

        int edge_count = 0;
        while (pos < line.size() && line[pos] != ' ')
        {
          edge_count = edge_count * 10 + (line[pos++] - '0');
        }
        if (pos >= line.size()) continue;
        pos++;

        for (int i = 0; i < edge_count; ++i)
        {
          if (!std::getline(file, line)) break;
          if (line.empty())
          {
            --i;
            continue;
          }

          size_t edge_pos = 0;
          std::string from, to;
          Weight weight = 0;

          while (edge_pos < line.size() && line[edge_pos] != ' ')
          {
            from += line[edge_pos++];
          }
          if (edge_pos >= line.size()) continue;
          edge_pos++;

          while (edge_pos < line.size() && line[edge_pos] != ' ')
          {
            to += line[edge_pos++];
          }
          if (edge_pos >= line.size()) continue;
          edge_pos++;

          while (edge_pos < line.size() && line[edge_pos] != ' ')
          {
            weight = weight * 10 + (line[edge_pos++] - '0');
          }

          addEdge(graph_name, from, to, weight);
        }
      }
    }

    void printGraphs() const
    {
      if (graphs.empty())
      {
        std::cout << "\n";
        return;
      }

      std::vector< std::string > names;
      for (const auto& graph_pair: graphs)
      {
        names.push_back(graph_pair.first);
      }

      for (size_t i = 0; i < names.size(); ++i)
      {
        for (size_t j = i + 1; j < names.size(); ++j)
        {
          if (names[i] > names[j])
          {
            std::swap(names[i], names[j]);
          }
        }
      }

      for (const auto& name: names)
      {
        std::cout << name << "\n";
      }
    }

    void printVertexes(const std::string& graph_name) const
    {
      if (!graphExists(graph_name))
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }

      auto vertices = graphs.at(graph_name).vertices;
      sortVertices(vertices);

      if (vertices.empty())
      {
        std::cout << "\n";
        return;
      }
      for (const auto& vertex: vertices)
      {
        std::cout << vertex << "\n";
      }
    }

    void printOutbound(const std::string& graph_name, const Vertex& vertex) const
    {
      if (!graphExists(graph_name) || !vertexExists(graph_name, vertex))
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }

      std::unordered_map< Vertex, Weights > outbound;
      for (const auto& edge_weights: graphs.at(graph_name).edges)
      {
        if (edge_weights.first.first == vertex)
        {
          for (auto w: edge_weights.second)
          {
            outbound[edge_weights.first.second].push_back(w);
          }
        }
      }

      if (outbound.empty())
      {
        std::cout << "\n";
        return;
      }

      std::vector< Vertex > sorted_vertices;
      for (const auto& pair: outbound)
      {
        sorted_vertices.push_back(pair.first);
      }
      sortVertices(sorted_vertices);

      for (const auto& v: sorted_vertices)
      {
        auto weights = outbound.at(v);
        sortWeights(weights);
        std::cout << v;
        for (const auto& weight: weights)
        {
          std::cout << " " << weight;
        }
        std::cout << "\n";
      }
    }

    void printInbound(const std::string& graph_name, const Vertex& vertex) const
    {
      if (!graphExists(graph_name) || !vertexExists(graph_name, vertex))
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }

      std::unordered_map< Vertex, Weights > inbound;
      for (const auto& edge_weights: graphs.at(graph_name).edges)
      {
        if (edge_weights.first.second == vertex)
        {
          for (auto w: edge_weights.second)
          {
            inbound[edge_weights.first.first].push_back(w);
          }
        }
      }

      if (inbound.empty())
      {
        std::cout << "\n";
        return;
      }

      std::vector< Vertex > sorted_vertices;
      for (const auto& pair: inbound)
      {
        sorted_vertices.push_back(pair.first);
      }
      sortVertices(sorted_vertices);

      for (const auto& v: sorted_vertices)
      {
        auto weights = inbound.at(v);
        sortWeights(weights);
        std::cout << v;
        for (const auto& weight: weights)
        {
          std::cout << " " << weight;
        }
        std::cout << "\n";
      }
    }

    void bindEdge(const std::string& graph_name, const Vertex& from, const Vertex& to, Weight weight)
    {
      if (!graphExists(graph_name))
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }
      addEdge(graph_name, from, to, weight);
    }

    void cutEdge(const std::string& graph_name, const Vertex& from, const Vertex& to, Weight weight)
    {
      if (!graphExists(graph_name))
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }

      auto& edges = graphs[graph_name].edges;
      auto edge_it = edges.find({from, to});
      if (edge_it == edges.end())
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }

      bool found = false;
      for (size_t i = 0; i < edge_it->second.size(); ++i)
      {
        if (edge_it->second[i] == weight)
        {
          edge_it->second.erase(edge_it->second.begin() + static_cast< std::ptrdiff_t >(i));
          found = true;
          break;
        }
      }

      if (!found)
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }

      if (edge_it->second.empty())
      {
        edges.erase(edge_it);
      }
    }

    void createGraph(const std::string& graph_name, const std::vector< Vertex >& vertices)
    {
      if (graphExists(graph_name))
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }

      for (const auto& v: vertices)
      {
        addVertex(graph_name, v);
      }

      if (vertices.empty())
      {
        graphs[graph_name];
      }
    }

    void mergeGraphs(const std::string& new_name, const std::string& name1, const std::string& name2)
    {
      if (!graphExists(name1) || !graphExists(name2) || graphExists(new_name))
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }

      for (const auto& v: graphs[name1].vertices)
      {
        addVertex(new_name, v);
      }
      for (const auto& v: graphs[name2].vertices)
      {
        addVertex(new_name, v);
      }

      for (const auto& edge_weights: graphs[name1].edges)
      {
        for (auto w: edge_weights.second)
        {
          addEdge(new_name, edge_weights.first.first, edge_weights.first.second, w);
        }
      }
      for (const auto& edge_weights: graphs[name2].edges)
      {
        for (auto w: edge_weights.second)
        {
          addEdge(new_name, edge_weights.first.first, edge_weights.first.second, w);
        }
      }
    }

    void extractGraph(const std::string& new_name, const std::string& old_name, const std::vector< Vertex >& vertices)
    {
      if (!graphExists(old_name) || graphExists(new_name))
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }

      for (const auto& v: vertices)
      {
        if (!vertexExists(old_name, v))
        {
          std::cout << "<INVALID COMMAND>\n";
          return;
        }
      }

      for (const auto& v: vertices)
      {
        addVertex(new_name, v);
      }

      for (const auto& edge_weights: graphs[old_name].edges)
      {
        bool from_exists = false;
        bool to_exists = false;
        for (const auto& v: vertices)
        {
          if (v == edge_weights.first.first) from_exists = true;
          if (v == edge_weights.first.second) to_exists = true;
          if (from_exists && to_exists) break;
        }

        if (from_exists && to_exists)
        {
          for (auto w: edge_weights.second)
          {
            addEdge(new_name, edge_weights.first.first, edge_weights.first.second, w);
          }
        }
      }
    }
  };
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " filename\n";
    return 1;
  }

  try
  {
    asafov::GraphManager manager;
    manager.loadGraphsFromFile(argv[1]);

    std::string command;
    while (std::cin >> command)
    {
      if (command == "graphs")
      {
        manager.printGraphs();
      }
      else if (command == "vertexes")
      {
        std::string graph_name;
        if (std::cin >> graph_name)
        {
          manager.printVertexes(graph_name);
        }
        else
        {
          std::cout << "<INVALID COMMAND>\n";
        }
      }
      else if (command == "outbound")
      {
        std::string graph_name, vertex;
        if (std::cin >> graph_name >> vertex)
        {
          manager.printOutbound(graph_name, vertex);
        }
        else
        {
          std::cout << "<INVALID COMMAND>\n";
        }
      }
      else if (command == "inbound")
      {
        std::string graph_name, vertex;
        if (std::cin >> graph_name >> vertex)
        {
          manager.printInbound(graph_name, vertex);
        }
        else
        {
          std::cout << "<INVALID COMMAND>\n";
        }
      }
      else if (command == "bind")
      {
        std::string graph_name, from, to;
        asafov::Weight weight;
        if (std::cin >> graph_name >> from >> to >> weight)
        {
          manager.bindEdge(graph_name, from, to, weight);
        }
        else
        {
          std::cout << "<INVALID COMMAND>\n";
        }
      }
      else if (command == "cut")
      {
        std::string graph_name, from, to;
        asafov::Weight weight;
        if (std::cin >> graph_name >> from >> to >> weight)
        {
          manager.cutEdge(graph_name, from, to, weight);
        }
        else
        {
          std::cout << "<INVALID COMMAND>\n";
        }
      }
      else if (command == "create")
      {
        std::string graph_name;
        if (std::cin >> graph_name)
        {
          std::vector< asafov::Vertex > vertices;
          asafov::Vertex v;
          while (std::cin >> v)
          {
            vertices.push_back(v);
          }
          manager.createGraph(graph_name, vertices);
        }
        else
        {
          std::cout << "<INVALID COMMAND>\n";
        }
      }
      else if (command == "merge")
      {
        std::string new_name, name1, name2;
        if (std::cin >> new_name >> name1 >> name2)
        {
          manager.mergeGraphs(new_name, name1, name2);
        }
        else
        {
          std::cout << "<INVALID COMMAND>\n";
        }
      }
      else if (command == "extract")
      {
        std::string new_name, old_name;
        int count;
        if (std::cin >> new_name >> old_name >> count)
        {
          std::vector< asafov::Vertex > vertices;
          asafov::Vertex v;
          for (int i = 0; i < count && std::cin >> v; ++i)
          {
            vertices.push_back(v);
          }
          if (static_cast< int >(vertices.size()) == count)
          {
            manager.extractGraph(new_name, old_name, vertices);
          }
          else
          {
            std::cout << "<INVALID COMMAND>\n";
          }
        }
        else
        {
          std::cout << "<INVALID COMMAND>\n";
        }
      }
      else
      {
        std::cout << "<INVALID COMMAND>\n";
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
