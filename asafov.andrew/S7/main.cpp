#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <memory>

namespace asafov
{
  using Vertex = std::string;
  using Weight = unsigned int;
  using Edge = std::pair< Vertex, Vertex >;
  using Weights = std::multiset< Weight >;

  struct Graph
  {
    std::set< Vertex > vertices;
    std::map< Edge, Weights > edges;
  };

  class GraphManager
  {
  private:
    std::map< std::string, Graph > graphs;

    void addVertex(const std::string& graph_name, const Vertex& vertex)
    {
      graphs[graph_name].vertices.insert(vertex);
    }

    void addEdge(const std::string& graph_name, const Vertex& from, const Vertex& to, Weight weight)
    {
      graphs[graph_name].edges[{from, to}].insert(weight);
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
      return it->second.vertices.find(vertex) != it->second.vertices.end();
    }

  public:
    void loadGraphsFromFile(const std::string& filename)
    {
      std::ifstream file(filename);
      if (!file.is_open())
      {
        throw std::runtime_error("Cannot open file");
      }

      std::string line;
      while (std::getline(file, line))
      {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string graph_name;
        int edge_count;

        if (!(iss >> graph_name >> edge_count)) continue;

        for (int i = 0; i < edge_count; ++i)
        {
          std::string from, to;
          Weight weight;
          if (!std::getline(file, line)) break;
          if (line.empty())
          {
            --i;
            continue;
          }

          std::istringstream edge_iss(line);
          if (edge_iss >> from >> to >> weight)
          {
            addEdge(graph_name, from, to, weight);
          }
        }
      }
    }

    void printGraphs() const
    {
      for (const auto& graph_pair: graphs)
      {
        std::cout << graph_pair.first << '\n';
      }
    }

    void printVertexes(const std::string& graph_name) const
    {
      if (!graphExists(graph_name))
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }

      const auto& vertices = graphs.at(graph_name).vertices;
      for (const auto& vertex: vertices)
      {
        std::cout << vertex << '\n';
      }
    }

    void printOutbound(const std::string& graph_name, const Vertex& vertex) const
    {
      if (!graphExists(graph_name) || !vertexExists(graph_name, vertex))
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }

      std::map< Vertex, Weights > outbound;
      for (const auto& edge_weights: graphs.at(graph_name).edges)
      {
        if (edge_weights.first.first == vertex)
        {
          outbound[edge_weights.first.second].insert(
            edge_weights.second.begin(),
            edge_weights.second.end()
          );
        }
      }

      for (const auto& vertex_weights: outbound)
      {
        std::cout << vertex_weights.first;
        for (const auto& weight: vertex_weights.second)
        {
          std::cout << ' ' << weight;
        }
        std::cout << '\n';
      }
    }

    void printInbound(const std::string& graph_name, const Vertex& vertex) const
    {
      if (!graphExists(graph_name) || !vertexExists(graph_name, vertex))
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }

      std::map< Vertex, Weights > inbound;
      for (const auto& edge_weights: graphs.at(graph_name).edges)
      {
        if (edge_weights.first.second == vertex)
        {
          inbound[edge_weights.first.first].insert(
            edge_weights.second.begin(),
            edge_weights.second.end()
          );
        }
      }

      for (const auto& vertex_weights: inbound)
      {
        std::cout << vertex_weights.first;
        for (const auto& weight: vertex_weights.second)
        {
          std::cout << ' ' << weight;
        }
        std::cout << '\n';
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

      auto& weights = edge_it->second;
      auto weight_it = weights.find(weight);
      if (weight_it == weights.end())
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }

      weights.erase(weight_it);
      if (weights.empty())
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
    }

    void mergeGraphs(const std::string& new_name, const std::string& name1, const std::string& name2)
    {
      if (!graphExists(name1) || !graphExists(name2) || graphExists(new_name))
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }

      // Merge vertices
      graphs[new_name].vertices.insert(graphs[name1].vertices.begin(), graphs[name1].vertices.end());
      graphs[new_name].vertices.insert(graphs[name2].vertices.begin(), graphs[name2].vertices.end());

      // Merge edges
      for (const auto& edge_weights: graphs[name1].edges)
      {
        graphs[new_name].edges[edge_weights.first].insert(
          edge_weights.second.begin(),
          edge_weights.second.end()
        );
      }
      for (const auto& edge_weights: graphs[name2].edges)
      {
        graphs[new_name].edges[edge_weights.first].insert(
          edge_weights.second.begin(),
          edge_weights.second.end()
        );
      }
    }

    void extractGraph(const std::string& new_name, const std::string& old_name, const std::vector< Vertex >& vertices)
    {
      if (!graphExists(old_name) || graphExists(new_name))
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }

      // Check all vertices exist in old graph
      for (const auto& v: vertices)
      {
        if (!vertexExists(old_name, v))
        {
          std::cout << "<INVALID COMMAND>\n";
          return;
        }
      }

      // Add vertices to new graph
      for (const auto& v: vertices)
      {
        addVertex(new_name, v);
      }

      // Add edges between these vertices
      for (const auto& edge_weights: graphs[old_name].edges)
      {
        bool from_exists = std::find(vertices.begin(), vertices.end(), edge_weights.first.first) != vertices.end();
        bool to_exists = std::find(vertices.begin(), vertices.end(), edge_weights.first.second) != vertices.end();

        if (from_exists && to_exists)
        {
          graphs[new_name].edges[edge_weights.first].insert(
            edge_weights.second.begin(),
            edge_weights.second.end()
          );
        }
      }
    }
  };
} // namespace asafov

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

    std::string line;
    while (std::getline(std::cin, line))
    {
      if (line.empty()) continue;

      std::istringstream iss(line);
      std::string command;
      iss >> command;

      if (command == "graphs")
      {
        manager.printGraphs();
      }
      else if (command == "vertexes")
      {
        std::string graph_name;
        if (iss >> graph_name)
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
        if (iss >> graph_name >> vertex)
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
        if (iss >> graph_name >> vertex)
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
        unsigned int weight;
        if (iss >> graph_name >> from >> to >> weight)
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
        unsigned int weight;
        if (iss >> graph_name >> from >> to >> weight)
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
        std::vector< asafov::Vertex > vertices;
        asafov::Vertex v;
        while (iss >> v)
        {
          vertices.push_back(v);
        }
        if (!vertices.empty())
        {
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
        if (iss >> new_name >> name1 >> name2)
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
        if (iss >> new_name >> old_name >> count)
        {
          std::vector< asafov::Vertex > vertices;
          asafov::Vertex v;
          for (int i = 0; i < count && iss >> v; ++i)
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
      }
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }

  return 0;
}
