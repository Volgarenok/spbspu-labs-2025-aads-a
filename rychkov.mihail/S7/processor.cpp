#include <iostream>
#include <fstream>
#include <stdexcept>
#include <utility>
#include <set.hpp>
#include <map.hpp>
#include "processor.hpp"

rychkov::S7ParseProcessor::S7ParseProcessor(int argc, char** argv)
{
  if (argc != 2)
  {
    throw std::invalid_argument("wrong arguments count");
  }
  std::ifstream file(argv[1]);
  if (!file)
  {
    throw std::invalid_argument("failed to open file");
  }
  std::string name;
  while (file >> name)
  {
    if (map.find(name) != map.end())
    {
      throw std::logic_error("found graph duplicate");
    }
    outer_mapped_type& graph = map.insert({name, {}}).first->second;
    size_t count = 0;
    if (!(file >> count))
    {
      throw std::logic_error("failed to read graph size");
    }
    for (size_t i = 0; i < count; i++)
    {
      std::string from, to;
      unsigned weight = 0;
      if (!(file >> from >> to >> weight))
      {
        throw std::logic_error("failed to read edge");
      }
      graph.first[{from, to}].insert(weight);
      graph.second.insert(from);
      graph.second.insert(to);
    }
  }
}
bool rychkov::S7ParseProcessor::graphs(ParserContext& context)
{
  if (!eol(context.in))
  {
    return false;
  }
  if (map.empty())
  {
    context.out << '\n';
    return true;
  }
  Set< std::string > graphs;
  for (const outer_map::value_type& i: map)
  {
    graphs.insert(i.first);
  }
  for (const std::string& i: graphs)
  {
    context.out << i << '\n';
  }
  return true;
}
bool rychkov::S7ParseProcessor::vertexes(ParserContext& context)
{
  std::string name;
  if (!(context.in >> name))
  {
    return false;
  }
  const vertexes_set& points = map.at(name).second;
  if (!eol(context.in))
  {
    return false;
  }
  if (points.empty())
  {
    context.out << '\n';
  }
  for (const std::string& i: points)
  {
    context.out << i << '\n';
  }
  return true;
}
bool rychkov::S7ParseProcessor::outbound(ParserContext& context)
{
  std::string name, point;
  if (!(context.in >> name >> point))
  {
    return false;
  }
  const outer_mapped_type& graph = map.at(name);
  if ((!graph.second.contains(point)) || !eol(context.in))
  {
    return false;
  }
  Map< std::string, weights_set > edges;
  for (const inner_map::value_type& i: graph.first)
  {
    if (i.first.first == point)
    {
      edges[i.first.second].insert(i.second.begin(), i.second.end());
    }
  }
  if (edges.empty())
  {
    context.out << '\n';
  }
  for (const decltype(edges)::value_type& i: edges)
  {
    context.out << i.first;
    for (unsigned j: i.second)
    {
      context.out << ' ' << j;
    }
    context.out << '\n';
  }
  return true;
}
bool rychkov::S7ParseProcessor::inbound(ParserContext& context)
{
  std::string name, point;
  if (!(context.in >> name >> point))
  {
    return false;
  }
  const outer_mapped_type& graph = map.at(name);
  if ((!graph.second.contains(point)) || !eol(context.in))
  {
    return false;
  }
  Map< std::string, weights_set > edges;
  for (const inner_map::value_type& i: graph.first)
  {
    if (i.first.second == point)
    {
      edges[i.first.first].insert(i.second.begin(), i.second.end());
    }
  }
  if (edges.empty())
  {
    context.out << '\n';
  }
  for (const decltype(edges)::value_type& i: edges)
  {
    context.out << i.first;
    for (unsigned j: i.second)
    {
      context.out << ' ' << j;
    }
    context.out << '\n';
  }
  return true;
}
bool rychkov::S7ParseProcessor::bind(ParserContext& context)
{
  std::string name, from, to;
  unsigned weight = 0;
  if (!(context.in >> name >> from >> to >> weight))
  {
    return false;
  }
  outer_mapped_type& graph = map.at(name);
  if (!eol(context.in))
  {
    return false;
  }
  graph.first[{from, to}].insert(weight);
  graph.second.insert(from);
  graph.second.insert(to);
  return true;
}
bool rychkov::S7ParseProcessor::cut(ParserContext& context)
{
  std::string name, from, to;
  unsigned weight = 0;
  if (!(context.in >> name >> from >> to >> weight))
  {
    return false;
  }
  inner_map& graph = map.at(name).first;
  if (!eol(context.in))
  {
    return false;
  }
  inner_map::iterator to_erase = graph.find({from, to});
  if (to_erase == graph.end())
  {
    return false;
  }
  weights_set::iterator i = to_erase->second.find(weight);
  if (i == to_erase->second.end())
  {
    return false;
  }
  to_erase->second.erase(i);
  if (to_erase->second.empty())
  {
    graph.erase(to_erase);
  }
  return true;
}
bool rychkov::S7ParseProcessor::create(ParserContext& context)
{
  std::string name;
  size_t count = 0;
  if (!(context.in >> name) || (map.find(name) != map.end()))
  {
    return false;
  }
  if ((context.in.peek() != ' ') || !context.in.ignore(1))
  {
    return false;
  }
  if ((context.in.peek() < '0') || (context.in.peek() > '9') || !(context.in >> count))
  {
    return false;
  }
  outer_mapped_type graph;
  std::string temp;
  for (size_t i = 0; i < count; i++)
  {
    if (!(context.in >> temp))
    {
      return false;
    }
    graph.second.insert(std::move(temp));
  }
  if (!eol(context.in))
  {
    return false;
  }
  map.insert({name, std::move(graph)});
  return true;
}
bool rychkov::S7ParseProcessor::merge(ParserContext& context)
{
  std::string result, lhs, rhs;
  if (!(context.in >> result >> lhs >> rhs) || (map.find(result) != map.end()) || !eol(context.in))
  {
    return false;
  }
  outer_mapped_type& left = map.at(lhs), right = map.at(rhs);
  outer_mapped_type& res = map.insert({result, left}).first->second;
  for (const inner_map::value_type& i: right.first)
  {
    res.first[i.first].insert(i.second.begin(), i.second.end());
  }
  for (const vertexes_set::value_type& i: right.second)
  {
    res.second.insert(i);
  }
  return true;
}
bool rychkov::S7ParseProcessor::extract(ParserContext& context)
{
  std::string result, rhs;
  size_t count = 0;
  if (!(context.in >> result >> rhs >> count) || (map.find(result) != map.end()))
  {
    return false;
  }
  outer_mapped_type graph;
  outer_mapped_type& source = map.at(rhs);
  std::string temp;
  for (size_t i = 0; i < count; i++)
  {
    if (!(context.in >> temp) || (source.second.find(temp) == source.second.end()))
    {
      return false;
    }
    graph.second.insert(std::move(temp));
  }
  if (!eol(context.in))
  {
    return false;
  }
  for (const inner_map::value_type& i: source.first)
  {
    if (graph.second.contains(i.first.first) && graph.second.contains(i.first.second))
    {
      graph.first.insert(i);
    }
  }
  map.insert({result, std::move(graph)});
  return true;
}
