#include "commands.hpp"

void krylov::printGraphs(std::ostream& out, const graphsTree& graphs)
{
  if (graphs.empty())
  {
    out << "\n";
    return;
  }
  for (auto it = graphs.cbegin(); it != graphs.cend(); ++it)
  {
    out << it->first << "\n";
  }
}

void krylov::vertexes(std::istream& in, std::ostream& out, const graphsTree& graphs)
{
  std::string name;
  in >> name;
  if (graphs.find(name) == graphs.cend())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  std::set< std::string > vertexes = graphs.at(name).getVertexes();
  if (vertexes.empty())
  {
    out << "\n";
    return;
  }
  for (auto it = vertexes.cbegin(); it != vertexes.cend(); ++it)
  {
    out << *it << "\n";
  }
}

void krylov::outBound(std::istream& in, std::ostream& out, const graphsTree& graphs)
{
  std::string name, vert;
  in >> name >> vert;
  if (graphs.find(name) == graphs.cend())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Graph gr = graphs.at(name);
  if (!gr.hasVert(vert))
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  std::map< std::string, std::map< size_t, size_t > > vertTree  = gr.getOutBound(vert);
  if (vertTree.empty())
  {
    out << "\n";
    return;
  }
  for (auto iter = vertTree.cbegin(); iter != vertTree.cend(); ++iter)
  {
    if (!iter->second.empty())
    {
      out << iter->first;
      for (auto iter2 = iter->second.cbegin(); iter2 != iter->second.cend(); ++iter2)
      {
        for (size_t i = 0; i < iter2->second; ++i)
        {
          out << " " << iter2->first;
        }
      }
      out << "\n";
    }
  }
}

void krylov::inBound(std::istream& in, std::ostream& out, const graphsTree& graphs)
{
  std::string name, vert;
  in >> name >> vert;
  Graph gr = graphs.at(name);
  if (!gr.hasVert(vert))
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  std::map< std::string, std::map< size_t, size_t > > vertTree  = gr.getInBound(vert);
  if (vertTree.empty())
  {
    out << "\n";
    return;
  }
  for (auto iter = vertTree.cbegin(); iter != vertTree.cend(); ++iter)
  {
    if (!iter->second.empty())
    {
      out << iter->first;
      for (auto iter2 = iter->second.cbegin(); iter2 != iter->second.cend(); ++iter2)
      {
        for (size_t i = 0; i < iter2->second; ++i)
        {
          out << " " << iter2->first;
        }
      }
      out << "\n";
    }
  }
}

void krylov::bind(std::istream& in, graphsTree& graphs)
{
  std::string name, vert1, vert2;
  size_t weight = 0;
  if (!(in >> name >> vert1 >> vert2 >> weight))
  {
    throw std::invalid_argument("<INVALID ARGUMENT>");
  }
  graphs.at(name).addEdge(vert1, vert2, weight);
}


void krylov::cut(std::istream& in, graphsTree& graphs)
{
  std::string name, vert1, vert2;
  size_t weight = 0;
  if (!(in >> name >> vert1 >> vert2 >> weight))
  {
    throw std::invalid_argument("<INVALID ARGUMENT>");
  }
  if (!graphs.at(name).removeEdge(vert1, vert2, weight))
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
}

void krylov::create(std::istream& in, graphsTree& graphs)
{
  std::string name;
  size_t count = 0;
  if (!(in >> name))
  {
    throw std::invalid_argument("<INVALID ARGUMENT>");
  }
  if (graphs.find(name) != graphs.end())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  in >> count;
  Graph gr;
  for (size_t i = 0; i < count; ++ i)
  {
    std::string vert;
    in >> vert;
    gr.addEdge(vert, vert, 0);
  }
  graphs[name] = gr;
}

void krylov::merge(std::istream& in, graphsTree& graphs)
{
  std::string newGraph, firstGraph, secondGraph;
  if (!(in >> newGraph >> firstGraph >> secondGraph))
  {
    throw std::invalid_argument("<INVALID ARGUMENT>");
  }
  if (graphs.find(newGraph) != graphs.end())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Graph gr;
  gr.addEdges(graphs.at(firstGraph));
  gr.addEdges(graphs.at(secondGraph));
  graphs[newGraph] = gr;
}

void krylov::extract(std::istream& in, graphsTree& graphs)
{
  std::string newGraph, firstGraph;
  size_t count = 0;
  if (!(in >> newGraph >> firstGraph >> count))
  {
    throw std::invalid_argument("<INVALID INPUT>");
  }
  if (graphs.find(newGraph) != graphs.end())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Graph graph1 = graphs.at(firstGraph);
  Graph gr;
  std::map< std::string, bool > vert;
  for (size_t i = 0; i < count; ++i)
  {
    std::string vert1;
    in >> vert1;
    if (!graph1.hasVert(vert1))
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    vert[vert1];
  }
  auto edges = graph1.getEdges();
  for (auto iter = edges.begin(); iter != edges.end(); ++iter)
  {
    if (vert.find(iter->first.first) != vert.end() && vert.find(iter->first.second) != vert.end())
    {
      for (auto iter2 = iter->second.cbegin(); iter2 != iter->second.cend(); ++iter2)
      {
        for (size_t i = 0; i < iter2->second; ++i)
        {
          gr.addEdge(iter->first.first, iter->first.second, iter2->first);
        }
      }
    }
  }
  graphs[newGraph] = gr;
}
