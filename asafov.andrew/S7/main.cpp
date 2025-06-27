#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <cctype>
#include <limits>
#include <functional>

namespace asafov
{
  using Vertex = std::string;
  using Weight = unsigned int;
  using Edge = std::pair< Vertex, Vertex >;
  using Weights = std::vector< Weight >;

  struct EdgeHash
  {
    std::size_t operator()(const Edge& e) const
    {
      std::size_t h1 = std::hash< Vertex >()(e.first);
      std::size_t h2 = std::hash< Vertex >()(e.second);
      return h1 ^ (h2 << 1);
    }
  };

  struct EdgeEqual
  {
    bool operator()(const Edge& lhs, const Edge& rhs) const
    {
      return lhs.first == rhs.first && lhs.second == rhs.second;
    }
  };

  struct Graph
  {
    std::vector< Vertex > vertices;
    std::unordered_map< Edge, Weights, EdgeHash, EdgeEqual > edges;

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

    bool readToken(std::istream& is, std::string& token)
    {
      token.clear();
      char c;
      while (is.get(c))
      {
        if (std::isspace(c))
        {
          if (!token.empty()) return true;
          continue;
        }
        token += c;
      }
      return !token.empty();
    }

    bool readNumber(std::istream& is, Weight& num)
    {
      num = 0;
      char c;
      bool hasDigit = false;
      while (is.get(c))
      {
        if (std::isdigit(c))
        {
          num = num * 10 + (c - '0');
          hasDigit = true;
        }
        else if (std::isspace(c))
        {
          return hasDigit;
        }
        else
        {
          is.unget();
          return false;
        }
      }
      return hasDigit;
    }

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
        size_t min_idx = i;
        for (size_t j = i + 1; j < weights.size(); ++j)
        {
          if (weights[j] < weights[min_idx])
          {
            min_idx = j;
          }
        }
        if (min_idx != i)
        {
          std::swap(weights[i], weights[min_idx]);
        }
      }
    }

    void sortVertices(std::vector< Vertex >& vertices) const
    {
      for (size_t i = 0; i < vertices.size(); ++i)
      {
        size_t min_idx = i;
        for (size_t j = i + 1; j < vertices.size(); ++j)
        {
          if (vertices[j] < vertices[min_idx])
          {
            min_idx = j;
          }
        }
        if (min_idx != i)
        {
          std::swap(vertices[i], vertices[min_idx]);
        }
      }
    }

    void sortVertexWeightPairs(std::vector< std::pair< Vertex, Weights > >& pairs) const
    {
      for (size_t i = 0; i < pairs.size(); ++i)
      {
        size_t min_idx = i;
        for (size_t j = i + 1; j < pairs.size(); ++j)
        {
          if (pairs[j].first < pairs[min_idx].first)
          {
            min_idx = j;
          }
        }
        if (min_idx != i)
        {
          std::swap(pairs[i], pairs[min_idx]);
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

      std::string graph_name;
      while (readToken(file, graph_name))
      {
        Weight edge_count;
        if (!readNumber(file, edge_count)) break;

        for (Weight i = 0; i < edge_count; ++i)
        {
          std::string from, to;
          Weight weight;
          if (!readToken(file, from)) break;
          if (!readToken(file, to)) break;
          if (!readNumber(file, weight)) break;

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
        size_t min_idx = i;
        for (size_t j = i + 1; j < names.size(); ++j)
        {
          if (names[j] < names[min_idx])
          {
            min_idx = j;
          }
        }
        if (min_idx != i)
        {
          std::swap(names[i], names[min_idx]);
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

      std::vector< std::pair< Vertex, Weights > > outbound;
      for (const auto& edge_weights: graphs.at(graph_name).edges)
      {
        if (edge_weights.first.first == vertex)
        {
          outbound.emplace_back(edge_weights.first.second, edge_weights.second);
        }
      }

      if (outbound.empty())
      {
        std::cout << "\n";
        return;
      }

      sortVertexWeightPairs(outbound);
      for (const auto& pair: outbound)
      {
        const auto& v = pair.first;
        auto sorted_weights = pair.second;
        sortWeights(sorted_weights);
        std::cout << v;
        for (const auto& weight: sorted_weights)
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

      std::vector< std::pair< Vertex, Weights > > inbound;
      for (const auto& edge_weights: graphs.at(graph_name).edges)
      {
        if (edge_weights.first.second == vertex)
        {
          inbound.emplace_back(edge_weights.first.first, edge_weights.second);
        }
      }

      if (inbound.empty())
      {
        std::cout << "\n";
        return;
      }

      sortVertexWeightPairs(inbound);
      for (const auto& pair: inbound)
      {
        const auto& v = pair.first;
        auto sorted_weights = pair.second;
        sortWeights(sorted_weights);
        std::cout << v;
        for (const auto& weight: sorted_weights)
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

      graphs[graph_name] = Graph();
      for (const auto& v: vertices)
      {
        addVertex(graph_name, v);
      }
    }

    void mergeGraphs(const std::string& new_name, const std::string& name1, const std::string& name2)
    {
      if (!graphExists(name1) || !graphExists(name2) || graphExists(new_name))
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }

      graphs[new_name] = Graph();

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

      graphs[new_name] = Graph();

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
        int count;
        if (std::cin >> graph_name >> count)
        {
          if (count < 0)
          {
            std::cout << "<INVALID COMMAND>\n";
            std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
            continue;
          }
          std::vector< asafov::Vertex > vertices;
          asafov::Vertex v;
          for (int i = 0; i < count && std::cin >> v; ++i)
          {
            vertices.push_back(v);
          }
          if (static_cast< int >(vertices.size()) == count)
          {
            manager.createGraph(graph_name, vertices);
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
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
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
