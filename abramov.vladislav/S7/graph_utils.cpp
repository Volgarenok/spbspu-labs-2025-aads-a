#include "graph_utils.hpp"

namespace
{
  using namespace abramov;

  using Arr = const SimpleArray< std::string >;
  using HashT = HashTable< std::string, SimpleArray< size_t > >;

  void printVertsWeights(Arr &vert_names, HashT &dest_verts, std::ostream &out)
  {
    for (size_t i = 0; i < vert_names.size(); ++i)
    {
      const std::string &vert = vert_names[i];
      out << vert;
      SimpleArray< size_t > &weights = dest_verts.find(vert)->second;
      if (weights.size() > 0)
      {
       std::sort(&weights[0], &weights[0] + weights.size());
      }
      for (size_t j = 0; j < weights.size(); ++j)
      {
        out << " " << weights[j];
      }
      out << "\n";
    }
  }

  void addEdgesVertexes(const Graph &graph, Graph &res)
  {
    for (auto it = graph.cbegin(); it != graph.cend(); ++it)
    {
      res.addVertex(it->first);
    }
    for (auto it = graph.cbegin(); it != graph.cend(); ++it)
    {
      const SimpleArray< Edge > *edges = graph.getEdges(it->first);
      if (edges)
      {
        for (size_t i = 0; i < edges->size(); ++i)
        {
          const Edge &edge = (*edges)[i];
          res.addEdge(edge.from, edge.to, edge.weight);
        }
      }
    }
  }
}


std::ifstream &abramov::readGraphs(std::ifstream &in, GraphCollection &collect)
{
  std::string name;
  size_t edges_count = 0;
  while(!in.eof())
  {
    if(!(in >> name >> edges_count))
    {
      break;
    }
    Graph graph(name);
    for (size_t i = 0; i < edges_count; ++i)
    {
      std::string vert_a;
      std::string vert_b;
      size_t w = 0;
      in >> vert_a >> vert_b >> w;
      graph.addEdge(vert_a, vert_b, w);
    }
    collect.addGraph(graph);
  }
  return in;
}

void abramov::printVertsSorted(const GraphCollection &collect, const std::string &name, std::ostream &out)
{
  const Graph &graph = collect.cgetGraph(name);
  SimpleArray< std::string > verts;
  HashTable< std::string, bool > uniq_verts;
  for (auto it = graph.cbegin(); it != graph.cend(); ++it)
  {
    const std::string &vert = it->first;
    if (uniq_verts.cfind(vert) == uniq_verts.cend())
    {
      uniq_verts.insert(vert, true);
      verts.pushBack(vert);
    }
    const SimpleArray< Edge > *edges = graph.getEdges(vert);
    if (edges)
    {
      for (size_t i = 0; i < edges->size(); ++i)
      {
        const std::string &vert = (*edges)[i].to;
        if (uniq_verts.cfind(vert) == uniq_verts.cend())
        {
          uniq_verts.insert(vert, true);
          verts.pushBack(vert);
        }
      }
    }
  }
  if (!verts.empty())
  {
    std::sort(&verts[0], &verts[0] + verts.size());
  }
  for (size_t i = 0; i < verts.size(); ++i)
  {
    out << verts[i] << "\n";
  }
}

void abramov::printOutVerts(const GraphCollection &collect, const std::string &n, const std::string &v, std::ostream &out)
{
  const Graph &graph = collect.cgetGraph(n);
  const SimpleArray< Edge > *edges = graph.getEdges(v);
  if (!edges)
  {
    throw std::logic_error("There is no such vertex\n");
  }
  if (edges->empty())
  {
    return;
  }
  HashTable< std::string, SimpleArray< size_t > > dest_verts;
  for (size_t i = 0; i < edges->size(); ++i)
  {
    const Edge &edge = (*edges)[i];
    auto it = dest_verts.find(edge.to);
    if (it != dest_verts.end())
    {
      it->second.pushBack(edge.weight);
    }
    else
    {
      SimpleArray< size_t > weights;
      weights.pushBack(edge.weight);
      dest_verts.insert(edge.to, weights);
    }
  }
  SimpleArray< std::string > vert_names;
  for (auto it = dest_verts.begin(); it != dest_verts.end(); ++it)
  {
    vert_names.pushBack(it->first);
  }
  if (!vert_names.empty())
  {
    std::sort(&vert_names[0], &vert_names[0] + vert_names.size());
  }
  printVertsWeights(vert_names, dest_verts, out);
}

void abramov::printInVerts(const GraphCollection &collect, const std::string &n, const std::string &v, std::ostream &out)
{
  const Graph &graph = collect.cgetGraph(n);
  HashTable< std::string, SimpleArray< size_t > > src_verts;
  for (auto it = graph.cbegin(); it != graph.cend(); ++it)
  {
    const SimpleArray< Edge > *edges = graph.getEdges(it->first);
    for (size_t i = 0; i < edges->size(); ++i)
    {
      const Edge &edge = (*edges)[i];
      if (edge.to == v)
      {
        auto srcIt = src_verts.find(edge.from);
        if (srcIt != src_verts.end())
        {
          srcIt->second.pushBack(edge.weight);
        }
      }
      else
      {
        SimpleArray< size_t > weights;
        weights.pushBack(edge.weight);
        src_verts.insert(edge.from, weights);
      }
    }
  }
  SimpleArray< std::string > vert_names;
  for (auto it = src_verts.begin(); it != src_verts.end(); ++it)
  {
    vert_names.pushBack(it->first);
  }
  if (!vert_names.empty())
  {
    std::sort(&vert_names[0], &vert_names[0] + vert_names.size());
  }
  printVertsWeights(vert_names, src_verts, out);
}

void abramov::addEdgeToGraph(GraphCollection &collect, const std::string &name, std::istream &in)
{
  Graph &graph = collect.getGraph(name);
  std::string from;
  std::string to;
  size_t w;
  in >> from >> to >> w;
  if (in)
  {
    graph.addEdge(from, to, w);
  }
  else
  {
    throw std::logic_error("Invalid arguments\n");
  }
}

void abramov::cutEdgeFromGraph(GraphCollection &collect, const std::string &name, std::istream &in)
{
  Graph &graph = collect.getGraph(name);
  std::string from;
  std::string to;
  size_t w;
  if (!(in >> from >> to >> w))
  {
    throw std::logic_error("Invalid arguments\n");
  }
  if (!graph.cutEdge(from, to, w))
  {
    throw std::logic_error("There is no such edge\n");
  }
}

void abramov::createGraph(GraphCollection &collect, std::istream &in)
{
  std::string name;
  in >> name;
  Graph res(name);
  try
  {
    res = collect.cgetGraph(name);
    throw std::runtime_error("There is such graph\n");
  }
  catch (const std::logic_error &)
  {}
  size_t k = 0;
  if (in.peek() == '\n')
  {
    throw std::logic_error("Invalid argument\n");
  }
  if (!(in >> k))
  {
    throw std::logic_error("Invalid argument\n");
  }
  if (!k)
  {
    collect.addGraph(res);
    return;
  }
  std::string vert;
  for (size_t i = 0; i < k; ++i)
  {
    if (!(in >> vert))
    {
      throw std::logic_error("Invalid argument\n");
    }
    res.addVertex(vert);
  }
  collect.addGraph(res);
}

void abramov::mergeGraphs(GraphCollection &collect, const std::string &name, std::istream &in)
{
  std::string n1;
  std::string n2;
  in >> n1 >> n2;
  const Graph &graph1 = collect.cgetGraph(n1);
  const Graph &graph2 = collect.cgetGraph(n2);
  Graph res(name);
  addEdgesVertexes(graph1, res);
  addEdgesVertexes(graph2, res);
  collect.addGraph(res);
}

void extractGraph(GraphCollection &collect, const std::string &name, std::istream in)
{
  std::string new_name;
  in >> new_name;
  size_t k = 0;
  in >> k;
  SimpleArray< std::string > verts;
  for (size_t i = 0; i < k; ++i)
  {
    std::string vert;
    in >> vert;
    verts.pushBack(vert);
  }
  const Graph &src_graph = collect.cgetGraph(name);
  for (size_t i = 0; i < verts.size(); ++i)
  {
    if (!src_graph.hasVertex(verts[i]))
    {
      throw std::logic_error("There is no such vertex\n");
    }
  }
  Graph res(new_name);
  for (size_t i = 0; i < verts.size(); ++i)
  {
    const SimpleArray< Edge > *edges = src_graph.getEdges(verts[i]);
    if (edges)
    {
      for (size_t j = 0; j < edges->size(); ++j)
      {
        for (size_t n = 0; n < verts.size(); ++n)
        {
          const Edge &edge = (*edges)[j];
          if (verts[n] == edge.to)
          {
            res.addEdge(edge.from, edge.to, edge.weight);
            break;
          }
        }
      }
    }
  }
  collect.addGraph(res);
}
