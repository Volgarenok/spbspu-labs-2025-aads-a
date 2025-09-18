#include "commands.hpp"

void finaev::printGraphs(std::ostream& out, const graphsTree& graphs)
{
  if (graphs.empty())
  {
    out << "\n";
    return;
  }
  for (auto it = graphs.cBegin(); it != graphs.cEnd(); ++it)
  {
    out << it->first << "\n";
  }
}

void finaev::vertexes(std::istream& in, std::ostream& out, const graphsTree& graphs)
{
  std::string name;
  in >> name;
  if (graphs.find(name) == graphs.cEnd())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  AVLtree< std::string, bool > vertexes = graphs.at(name).getVertexes();
  if (vertexes.empty())
  {
    out << "\n";
    return;
  }
  for (auto it = vertexes.cBegin(); it != vertexes.cEnd(); ++it)
  {
    out << it->first << "\n";
  }
}

void finaev::outBound(std::istream& in, std::ostream& out, const graphsTree& graphs)
{
  std::string name, vert;
  in >> name >> vert;
  if (graphs.find(name) == graphs.cEnd())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Graph gr = graphs.at(name);
  if (!gr.hasVert(vert))
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  AVLtree< std::string, AVLtree< size_t, size_t > > vertTree  = gr.getOutBound(vert);
  if (vertTree.empty())
  {
    out << "\n";
    return;
  }
  for (auto iter = vertTree.cBegin(); iter != vertTree.cEnd(); ++iter)
  {
    if (!iter->second.empty())
    {
      out << iter->first;
      for (auto iter2 = iter->second.cBegin(); iter2 != iter->second.cEnd(); ++iter2)
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

void finaev::inBound(std::istream& in, std::ostream& out, const graphsTree& graphs)
{
  std::string name, vert;
  in >> name >> vert;
  Graph gr = graphs.at(name);
  if (!gr.hasVert(vert))
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  AVLtree< std::string, AVLtree< size_t, size_t > > vertTree  = gr.getInBound(vert);
  if (vertTree.empty())
  {
    out << "\n";
    return;
  }
  for (auto iter = vertTree.cBegin(); iter != vertTree.cEnd(); ++iter)
  {
    if (!iter->second.empty())
    {
      out << iter->first;
      for (auto iter2 = iter->second.cBegin(); iter2 != iter->second.cEnd(); ++iter2)
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

void finaev::bind(std::istream& in, graphsTree& graphs)
{
  std::string name, vert1, vert2;
  size_t weight = 0;
  if (!(in >> name >> vert1 >> vert2 >> weight))
  {
    throw std::invalid_argument("<INVALID ARGUMENT>");
  }
  graphs.at(name).addEdge(vert1, vert2, weight);
}


void finaev::cut(std::istream& in, graphsTree& graphs)
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

void finaev::create(std::istream& in, graphsTree& graphs)
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

void finaev::merge(std::istream& in, graphsTree& graphs)
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

void finaev::extract(std::istream& in, graphsTree& graphs)
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
  AVLtree< std::string, bool > vert;
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
      for (auto iter2 = iter->second.cBegin(); iter2 != iter->second.cEnd(); ++iter2)
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

using cmdMap = finaev::AVLtree< std::string, std::function< void() > >;
cmdMap finaev::createCommandsHandler(std::istream& in, std::ostream& out, graphsTree& graphs)
{
  cmdMap commands;
  commands["graphs"] = std::bind(printGraphs, std::ref(out), std::cref(graphs));
  commands["vertexes"] = std::bind(vertexes, std::ref(in), std::ref(out), std::cref(graphs));
  commands["outbound"] = std::bind(outBound, std::ref(in), std::ref(out), std::cref(graphs));
  commands["inbound"] = std::bind(inBound, std::ref(in), std::ref(out), std::cref(graphs));
  commands["bind"] = std::bind(bind, std::ref(in), std::ref(graphs));
  commands["cut"] = std::bind(cut, std::ref(in), std::ref(graphs));
  commands["merge"] = std::bind(merge, std::ref(in), std::ref(graphs));
  commands["create"] = std::bind(create, std::ref(in), std::ref(graphs));
  commands["extract"] = std::bind(extract, std::ref(in), std::ref(graphs));
  return commands;
}
