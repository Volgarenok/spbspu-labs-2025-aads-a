#include "commands.hpp"

namespace
{
  using demehin::Tree;

  void printBounds(std::ostream& out, const Tree< std::string, Tree< unsigned int, size_t > > bounds)
  {
    if (bounds.size() == 0)
    {
      out << "\n";
      return;
    }

    for (auto it = bounds.begin(); it != bounds.end(); it++)
    {
      out << (*it).first;
      auto weights_it = (*it).second.begin();
      for(; weights_it != (*it).second.end(); weights_it++)
      {
        for (size_t i = 0; i < (*weights_it).second; i++)
        {
          out << " " << (*weights_it).first;
        }
      }
      out << "\n";
    }
  }

  void addEdges(demehin::Graph& gr, const demehin::Graph::Edges& edges)
  {
    for (auto it = edges.cbegin(); it != edges.cend(); it++)
    {
      for (auto&& weight: it->second)
      {
        gr.addEdge(it->first.first, it->first.second, weight);
      }
    }
  }
}

void demehin::printGraphsNames(std::ostream& out, const MapOfGraphs& graphs)
{
  if (graphs.empty())
  {
    out << "\n";
    return;
  }

  for (auto it = graphs.begin(); it != graphs.end(); it++)
  {
    out << (*it).first << "\n";
  }
}

void demehin::printVertexesNames(std::ostream& out, std::istream& in, const MapOfGraphs& graphs)
{
  std::string gr_name;
  in >> gr_name;
  Graph gr = graphs.at(gr_name);

  if (gr.getVrts().empty())
  {
    out << "\n";
    return;
  }

  demehin::Tree< std::string, std::string > vrts = gr.getVrts();
  for (auto it = vrts.begin(); it != vrts.end(); it++)
  {
    out << (*it).first << "\n";
  }
}

void demehin::printOutbounds(std::ostream& out, std::istream& in, const MapOfGraphs& graphs)
{
  std::string gr_name, vrt_name;
  in >> gr_name >> vrt_name;

  Graph gr = graphs.at(gr_name);

  if (!gr.hasVrt(vrt_name))
  {
    throw std::logic_error("incorrect parameters");
  }

  auto outbounds = gr.getOutbounds(vrt_name);
  printBounds(out, outbounds);
}

void demehin::printInbounds(std::ostream& out, std::istream& in, const MapOfGraphs& graphs)
{
  std::string gr_name, vrt_name;
  in >> gr_name >> vrt_name;

  Graph gr = graphs.at(gr_name);

  if (!gr.hasVrt(vrt_name))
  {
    throw std::logic_error("incorrect parameters");
  }

  auto inbounds = gr.getInbounds(vrt_name);
  printBounds(out, inbounds);
}

void demehin::bind(std::istream& in, MapOfGraphs& graphs)
{
  std::string gr_name, from, to;
  unsigned int weight;
  in >> gr_name >> from >> to >> weight;
  if (!in)
  {
    throw std::logic_error("incorrect input");
  }

  graphs.at(gr_name).addEdge(from, to, weight);
}

void demehin::cut(std::istream& in, MapOfGraphs& graphs)
{
  std::string gr_name, from, to;
  unsigned int weight;
  in >> gr_name >> from >> to >> weight;
  if (!in)
  {
    throw std::logic_error("incorrect input");
  }

  if (!(graphs.at(gr_name).deleteEdge(from, to, weight)))
  {
    throw std::logic_error("incorrect parameters");
  }
}

void demehin::create(std::istream& in, MapOfGraphs& graphs)
{

  std::string gr_name;
  in >> gr_name;

  if (graphs.find(gr_name) != graphs.end())
  {
    throw std::logic_error("incorrect parameters");
  }

  Graph newGraph;
  size_t vrt_cnt = 0;
  in >> vrt_cnt;
  if (!in)
  {
    throw std::logic_error("incorrect input");
  }

  for (size_t i = 0; i < vrt_cnt; i++)
  {
    std::string vrt;
    if (!(in >> vrt))
    {
      throw std::logic_error("incorrect parameter");
    }
    newGraph.addVertex(vrt);
  }
  graphs[gr_name] = newGraph;

}

void demehin::merge(std::istream& in, MapOfGraphs& graphs)
{
  std::string new_gr_name, gr1_name, gr2_name;
  in >> new_gr_name >> gr1_name >> gr2_name;
  auto end = graphs.end();
  if (graphs.find(gr1_name) == end || graphs.find(gr2_name) == end || graphs.find(new_gr_name) != end)
  {
    throw std::logic_error("incorrect parameters");
  }

  Graph res;

  auto g1 = graphs[gr1_name];
  auto edges1 = g1.getEdges();
  addEdges(res, edges1);

  auto g2 = graphs[gr2_name];
  auto edges2 = g2.getEdges();
  addEdges(res, edges2);

  graphs[new_gr_name] = res;
}

void demehin::extract(std::istream& in, MapOfGraphs& graphs)
{
  std::string new_gr_name, gr_name;
  size_t vrt_cnt;
  in >> new_gr_name >> gr_name >> vrt_cnt;
  if (graphs.find(new_gr_name) != graphs.end() || graphs.find(gr_name) == graphs.end())
  {
    throw std::logic_error("incorrect parameters");
  }

  List< std::string > vrts;
  auto gr = graphs.at(gr_name);
  for (size_t i = 0; i < vrt_cnt; i++)
  {
    std::string vrt;
    in >> vrt;
    if (!gr.hasVrt(vrt))
    {
      throw std::logic_error("incorrect parameters");
    }
    vrts.push_back(vrt);
  }

  Graph new_gr;

  for (auto it = gr.getEdges().cbegin(); it != gr.getEdges().cend(); it++)
  {
    std::string from = it->first.first;
    std::string to = it->first.second;
    bool from_found = false;
    bool to_found = false;
    for (auto&& v: vrts)
    {
      if (v == from)
      {
        from_found = true;
      }
      if (v == to)
      {
        to_found = true;
      }
    }

    if (from_found && to_found)
    {
      for (auto&& w: it->second)
      {
        new_gr.addEdge(from, to, w);
      }
    }
  }
  graphs[new_gr_name] = new_gr;
}
