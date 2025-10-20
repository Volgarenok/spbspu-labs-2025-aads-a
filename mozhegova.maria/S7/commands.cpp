#include "commands.hpp"

void mozhegova::printGraphs(std::ostream & out, const MapOfGraphs & graphs)
{
  if (graphs.empty())
  {
    out << '\n';
    return;
  }
  for (auto it = graphs.cbegin(); it != graphs.cend(); ++it)
  {
    out << it->first << '\n';
  }
}

void mozhegova::printVertexes(std::istream & in, std::ostream & out, const MapOfGraphs & graphs)
{
  std::string graphname;
  in >> graphname;
  BiTree< std::string, bool > vert = graphs.at(graphname).getVertexes();
  if (vert.empty())
  {
    out << '\n';
    return;
  }
  for (auto it = vert.cbegin(); it != vert.cend(); ++it)
  {
    out << it->first << '\n';
  }
}

void mozhegova::printOutbounds(std::istream & in, std::ostream & out, const MapOfGraphs & graphs)
{
  std::string graphname, vert;
  in >> graphname >> vert;
  BiTree< std::string, BiTree< size_t, size_t > > outbounds = graphs.at(graphname).getOutBounds(vert);
  if (outbounds.empty())
  {
    out << '\n';
    return;
  }
  for (auto it = outbounds.cbegin(); it != outbounds.cend(); ++it)
  {
    out << it->first;
    for (auto it2 = it->second.cbegin(); it2 != it->second.cend(); ++it2)
    {
      for (size_t i = 0; i < it2->second; ++i)
      {
        out << ' ' << it2->first;
      }
    }
    out << '\n';
  }
}

void mozhegova::printInbounds(std::istream & in, std::ostream & out, const MapOfGraphs & graphs)
{
  std::string graphname, vert;
  in >> graphname >> vert;
  BiTree< std::string, BiTree< size_t, size_t > > outbounds = graphs.at(graphname).getInBounds(vert);
  if (outbounds.empty())
  {
    out << '\n';
    return;
  }
  for (auto it = outbounds.cbegin(); it != outbounds.cend(); ++it)
  {
    out << it->first;
    for (auto it2 = it->second.cbegin(); it2 != it->second.cend(); ++it2)
    {
      for (size_t i = 0; i < it2->second; ++i)
      {
        out << ' ' << it2->first;
      }
    }
    out << '\n';
  }
}

void mozhegova::bind(std::istream & in, MapOfGraphs & graphs)
{
  std::string graphname, vert1, vert2;
  size_t weight = 0;
  in >> graphname >> vert1 >> vert2 >> weight;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  graphs.at(graphname).addEdge(vert1, vert2, weight);
}

void mozhegova::cut(std::istream & in, MapOfGraphs & graphs)
{
  std::string graphname, vert1, vert2;
  size_t weight = 0;
  in >> graphname >> vert1 >> vert2 >> weight;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  if (!graphs.at(graphname).removeEdge(vert1, vert2, weight))
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
}

void mozhegova::create(std::istream & in, MapOfGraphs & graphs)
{
  std::string graphname;
  in >> graphname;
  if (graphs.find(graphname) != graphs.end())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  size_t count = 0;
  if (!(in >> count))
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Graph temp;
  for (size_t i = 0; i < count; ++i)
  {
    std::string vert;
    in >> vert;
    temp.addVertex(vert);
  }
  graphs[graphname] = temp;
}

void mozhegova::merge(std::istream & in, MapOfGraphs & graphs)
{
  std::string newgraph, graphname1, graphname2;
  in >> newgraph >> graphname1 >> graphname2;
  auto it1 = graphs.find(graphname1);
  auto it2 = graphs.find(graphname2);
  if (it1 == graphs.end() || it2 == graphs.end())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  const Graph & graph1 = it1->second;
  const Graph & graph2 = it2->second;
  Graph temp = graph1;
  temp.addEdges(graph2);
  graphs[newgraph] = temp;
}

void mozhegova::extract(std::istream & in, MapOfGraphs & graphs)
{
  std::string newgraph, oldgraph;
  in >> newgraph >> oldgraph;
  if (graphs.find(newgraph) != graphs.end())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  size_t count = 0;
  if (!(in >> count))
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  const Graph & graph = graphs.at(oldgraph);
  Graph temp;
  BiTree< std::string, bool > verttemp;
  for (size_t i = 0; i < count; ++i)
  {
    std::string vert;
    in >> vert;
    if (!graph.hasVert(vert))
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    verttemp[vert] = true;
    temp.addVertex(vert);
  }
  auto edges = graph.getEdges();
  for (auto it = edges.begin(); it != edges.end(); ++it)
  {
    auto a = verttemp.find(it->first.first);
    auto b = verttemp.find(it->first.second);
    if (a != verttemp.end() && b != verttemp.end())
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
  graphs[newgraph] = temp;
}
