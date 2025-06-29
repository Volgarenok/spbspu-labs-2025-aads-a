#include "graph-commands.hpp"
#include <exception>

void alymova::GraphsCommand::operator()(const GraphSet& graphs)
{
  List< std::string > names;
  for (auto it = graphs.begin(); it != graphs.end(); it++)
  {
    names.push_back(it->first);
  }
  names.sort();
  out << names;
}

void alymova::VertexesCommand::operator()(const GraphSet& graphs)
{
  std::string name;
  in >> name;
  auto it_name = graphs.find(name);
  if (it_name == graphs.end() || !in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  List< std::string > copy_vertexes(it_name->second.vertexes);
  copy_vertexes.sort();
  out << copy_vertexes;
}

void alymova::OutboundCommand::operator()(const GraphSet& graphs)
{
  std::string graph_name, vertex;
  in >> graph_name >> vertex;
  auto it_name = graphs.find(graph_name);
  if (it_name == graphs.end() || !it_name->second.hasVertex(vertex) || !in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  BoundMap outbound = it_name->second.getOutbound(vertex);
  out << outbound;
}

void alymova::InboundCommand::operator()(const GraphSet& graphs)
{
  std::string graph_name, vertex;
  in >> graph_name >> vertex;
  auto it_name = graphs.find(graph_name);
  if (it_name == graphs.end() || !it_name->second.hasVertex(vertex) || !in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  BoundMap inbound = it_name->second.getInbound(vertex);
  out << inbound;
}

void alymova::BindCommand::operator()(GraphSet& graphs)
{
  std::string graph_name, vertex1, vertex2;
  size_t weight;
  in >> graph_name >> vertex1 >> vertex2 >> weight;
  auto it_name = graphs.find(graph_name);
  if (it_name == graphs.end() || !in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  it_name->second.addEdge(vertex1, vertex2, weight);
}

void alymova::CutCommand::operator()(GraphSet& graphs)
{
  std::string graph_name, vertex1, vertex2;
  size_t weight;
  in >> graph_name >> vertex1 >> vertex2 >> weight;
  auto it_name = graphs.find(graph_name);
  if (it_name == graphs.end() || !in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  it_name->second.cutEdge(vertex1, vertex2, weight);
}

void alymova::CreateCommand::operator()(GraphSet& graphs)
{
  std::string graph_name;
  in >> graph_name;
  if (graphs.find(graph_name) != graphs.end() || !in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  size_t vertexes_cnt;
  in >> vertexes_cnt;

  Graph graph_new;
  std::string vertex;
  for (size_t i = 0; i < vertexes_cnt; i++)
  {
    if (!(in >> vertex))
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    graph_new.addVertex(vertex);
  }
  graphs.insert(std::make_pair(graph_name, graph_new));
}

void alymova::MergeCommand::operator()(GraphSet& graphs)
{
  std::string name_new, name1, name2;
  in >> name_new >> name1 >> name2;
  auto it_name1 = graphs.find(name1);
  auto it_name2 = graphs.find(name2);
  if (graphs.find(name_new) != graphs.end() || it_name1 == graphs.end() || it_name2 == graphs.end() || !in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Graph graph_new = it_name1->second;
  graph_new.merge(it_name2->second);
  graphs.insert(std::make_pair(name_new, graph_new));
}

void alymova::ExtractCommand::operator()(GraphSet& graphs)
{
  std::string name_new, name;
  in >> name_new >> name;
  auto it_name = graphs.find(name);
  if (it_name == graphs.end() || graphs.find(name_new) != graphs.end() || !in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  size_t vertexes_cnt;
  in >> vertexes_cnt;

  Graph graph_new;
  std::string vertex;
  for (size_t i = 0; i < vertexes_cnt; i++)
  {
    if (!(in >> vertex) || !it_name->second.hasVertex(vertex))
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    graph_new.addVertex(vertex);
  }
  for (auto it = it_name->second.edges.begin(); it != it_name->second.edges.end(); it++)
  {
    if (graph_new.hasVertex(it->first.first) && graph_new.hasVertex(it->first.second))
    {
      graph_new.addEdge(it->first.first, it->first.second, it->second);
    }
  }
  graphs.insert(std::make_pair(name_new, graph_new));
}

alymova::GraphSet alymova::readGraphsFile(std::istream& in)
{
  GraphSet graphs;
  std::string name;
  while (in >> name)
  {
    Graph gr;
    if (in >> gr)
    {
      graphs.insert(std::make_pair(name, gr));
    }
  }
  if (!in.eof() && in.fail())
  {
    throw std::logic_error("<INCORRECT GRAPHS DESCRIPTION>");
  }
  return graphs;
}

alymova::CommandSet alymova::complectCommands(std::istream& in, std::ostream& out)
{
  return
  {
    {"graphs", GraphsCommand{out}},
    {"vertexes", VertexesCommand{in, out}},
    {"outbound", OutboundCommand{in, out}},
    {"inbound", InboundCommand{in, out}},
    {"bind", BindCommand{in}},
    {"cut", CutCommand{in}},
    {"create", CreateCommand{in}},
    {"merge", MergeCommand{in}},
    {"extract", ExtractCommand{in}}
  };
}
