#include "commands.hpp"
#include <AVLtree.hpp>

namespace tkach
{
  void printGraphs(std::ostream& out, const AvlTree< std::string, Graph >& tree_of_graphs)
  {
    if (tree_of_graphs.empty())
    {
      out << "\n";
      return;
    }
    for (auto it = tree_of_graphs.cbegin(); it != tree_of_graphs.cend(); ++it)
    {
      out << it->first << "\n";
    }
  }

  void printVertexes(std::ostream& out, std::istream& in, const AvlTree< std::string, Graph >& tree_of_graphs)
  {
    std::string name;
    in >> name;
    AvlTree< std::string, bool > tree_of_vert = tree_of_graphs.at(name).getAllVert();
    if (tree_of_vert.empty())
    {
      out << "\n";
      return;
    }
    for (auto it = tree_of_vert.cbegin(); it != tree_of_vert.cend(); ++it)
    {
      out << it->first << "\n";
    }
  }

  void printOutbound(std::ostream& out, std::istream& in, const AvlTree< std::string, Graph >& tree_of_graphs)
  {
    std::string name_graph, name_vert;
    in >> name_graph >> name_vert;
    Graph temp = tree_of_graphs.at(name_graph);
    if (!temp.hasVert(name_vert))
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    AvlTree< std::string, AvlTree< size_t, size_t > > tree_of_vert = temp.getOutBound(name_vert);
    if (tree_of_vert.empty())
    {
      out << "\n";
      return;
    }
    for (auto it = tree_of_vert.cbegin(); it != tree_of_vert.cend(); ++it)
    {
      if (!it->second.empty())
      {
        out << it->first;
        for (auto it2 = it->second.cbegin(); it2 != it->second.cend(); ++it2)
        {
          for (size_t i = 0; i < it2->second; ++i)
          {
            out << " " << it2->first;
          }
        }
        out << "\n";
      }
    }
  }

  void printInbound(std::ostream& out, std::istream& in, const AvlTree< std::string, Graph >& tree_of_graphs)
  {
    std::string name_graph, name_vert;
    in >> name_graph >> name_vert;
    Graph temp = tree_of_graphs.at(name_graph);
    if (!temp.hasVert(name_vert))
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    AvlTree< std::string, AvlTree< size_t, size_t > > tree_of_vert = temp.getInBound(name_vert);
    if (tree_of_vert.empty())
    {
      out << "\n";
      return;
    }
    for (auto it = tree_of_vert.cbegin(); it != tree_of_vert.cend(); ++it)
    {
      if (!it->second.empty())
      {
        out << it->first;
        for (auto it2 = it->second.cbegin(); it2 != it->second.cend(); ++it2)
        {
          for (size_t i = 0; i < it2->second; ++i)
          {
            out << " " << it2->first;
          }
        }
        out << "\n";
      }
    }
  }

  void bind(std::istream& in, AvlTree< std::string, Graph >& tree_of_graphs)
  {
    std::string name_graph, name_vert1, name_vert2;
    size_t weight = 0;
    if (!(in >> name_graph >> name_vert1 >> name_vert2 >> weight))
    {
      throw std::invalid_argument("<INVALID INPUT>");
    }
    tree_of_graphs.at(name_graph).addEdge(name_vert1, name_vert2, weight);
  }

  void cut(std::istream& in, AvlTree< std::string, Graph >& tree_of_graphs)
  {
    std::string name_graph, name_vert1, name_vert2;
    size_t weight = 0;
    if (!(in >> name_graph >> name_vert1 >> name_vert2 >> weight))
    {
      throw std::invalid_argument("<INVALID INPUT>");
    }
    if (!tree_of_graphs.at(name_graph).removeEdge(name_vert1, name_vert2, weight))
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
  }

  void create(std::istream& in, AvlTree< std::string, Graph >& tree_of_graphs)
  {
    std::string name_graph;
    size_t count = 0;
    in >> name_graph;
    if (tree_of_graphs.find(name_graph) != tree_of_graphs.end())
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    if (!(in >> count))
    {
      throw std::invalid_argument("<INVALID INPUT>");
    }
    Graph temp;
    for (size_t i = 0; i < count; ++ i)
    {
      std::string vert1;
      in >> vert1;
      temp.addEdge(vert1, vert1, 0);
    }
    tree_of_graphs[name_graph] = temp;
  }

  void merge(std::istream& in, AvlTree< std::string, Graph >& tree_of_graphs)
  {
    std::string new_graph, graph1, graph2;
    in >> new_graph >> graph1 >> graph2;
    if (tree_of_graphs.find(new_graph) != tree_of_graphs.end())
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    Graph temp;
    temp.addEdges(tree_of_graphs.at(graph1));
    temp.addEdges(tree_of_graphs.at(graph2));
    tree_of_graphs[new_graph] = temp;
  }

  void extract(std::istream& in, AvlTree< std::string, Graph >& tree_of_graphs)
  {
    std::string new_graph, name_graph1;
    size_t count = 0;
    if (!(in >> new_graph >> name_graph1 >> count))
    {
      throw std::invalid_argument("<INVALID INPUT>");
    }
    if (tree_of_graphs.find(new_graph) != tree_of_graphs.end())
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    Graph graph1 = tree_of_graphs.at(name_graph1);
    Graph temp;
    AvlTree< std::string, bool > need_vert;
    for (size_t i = 0; i < count; ++ i)
    {
      std::string vert1;
      in >> vert1;
      if (!graph1.hasVert(vert1))
      {
        throw std::logic_error("<INVALID COMMAND>");
      }
      need_vert[vert1];
    }
    auto edges = graph1.gerGraphEdges();
    for (auto it = edges.begin(); it != edges.end(); ++it)
    {
      if (need_vert.find(it->first.first) != need_vert.end() && need_vert.find(it->first.second) != need_vert.end())
      {
        for (auto it2 = it->second.cbegin(); it2 != it->second.cend(); ++it2)
        {
          for (size_t i = 0; i < it2->second; ++i)
          {
            temp.addEdge(it->first.first, it->first.second, it2->first);
          }
        }
      }
    }
    tree_of_graphs[new_graph] = temp;
  }
}
