#include "commands.hpp"
#include <algorithm>
#include <limits>

void dribas::graph(const RobinHoodHashTable< std::string, Graph >& graphs, std::ostream& out)
{
  std::vector< std::string > graphNames;
  for (const auto& pair: graphs) {
    graphNames.push_back(pair.first);
  }
  std::sort(graphNames.begin(), graphNames.end());
  for (const auto& name: graphNames) {
    out << name << '\n';
  }
  if (graphNames.empty()) {
    out << '\n';
  }
}

void dribas::vertexes(const RobinHoodHashTable< std::string, Graph >& graphs, std::istream& in, std::ostream& out)
{
  std::string name;
  in >> name;
  auto i = graphs.find(name);
  if (i == graphs.end()) {
    throw std::invalid_argument("");
  }
  auto vertexes = (*i).second.getUniqueVertexes();
  for (const auto& vertex: vertexes) {
    out << vertex << '\n';
  }
  if (vertexes.empty()) {
    out << '\n';
  }
}

void dribas::outbound(const RobinHoodHashTable< std::string, Graph >& graphs, std::istream& in, std::ostream& out)
{
  std::string name;
  std::string vertex;
  in >> name >> vertex;
  auto i = graphs.find(name);
  if (i == graphs.end()) {
    throw std::invalid_argument("");
  }
  if (!(*i).second.hasVertex(vertex)) {
    throw std::invalid_argument("");
  }
  auto vertexes = (*i).second.getOutboundEdges(vertex);
  for (const auto& faces: vertexes) {
    out << faces.first;
    for (const auto weights: faces.second) {
      out << ' ' << weights;
    }
    out << '\n';
  }
  if (vertexes.empty()) {
    out << '\n';
  }
}

void dribas::inbound(const RobinHoodHashTable< std::string, Graph >& graphs, std::istream& in, std::ostream& out)
{
  std::string name;
  std::string vertex;
  in >> name >> vertex;
  auto i = graphs.find(name);
  if (i == graphs.end()) {
    throw std::invalid_argument("");
  }
  if (!(*i).second.hasVertex(vertex)) {
    throw std::invalid_argument("");
  }
  auto vertexes = (*i).second.getInboundEdges(vertex);
  for (const auto& faces: vertexes) {
    out << faces.first;
    for (const auto weights: faces.second) {
      out << ' ' << weights;
    }
    out << '\n';
  }
  if (vertexes.empty()) {
    out << '\n';
  }
}

void dribas::bind(RobinHoodHashTable< std::string, Graph >& graphs, std::istream& in)
{
  std::string name;
  std::string from;
  std::string to;
  int weight = 0;
  in >> name >> from >> to >> weight;
  if (in.fail() && !in.eof()) {
    throw std::invalid_argument("");
  }
  auto it = graphs.find(name);
  if (it == graphs.end()) {
    throw std::invalid_argument("");
  }
  (*it).second.addEdge(from, to, weight);
}

void dribas::cut(RobinHoodHashTable< std::string, Graph >& graphs, std::istream& in)
{
  std::string name;
  std::string from;
  std::string to;
  int weight = 0;
  in >> name >> from >> to >> weight;
  if (in.fail() && !in.eof()) {
    throw std::invalid_argument("");
  }
  auto it = graphs.find(name);
  if (it == graphs.end()) {
    throw std::invalid_argument("");
  }
  if (!(*it).second.removeEdge(from, to, weight)) {
    throw std::invalid_argument("");
  }
}

void dribas::create(RobinHoodHashTable< std::string, Graph >& graphs, std::istream& in)
{
  std::string name;
  in >> name;
  if (graphs.find(name) != graphs.end()) {
    throw std::invalid_argument("");
  }
  size_t vertexCount;
  if (!(in >> vertexCount)) {
    in.clear();
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    graphs.insert(name, Graph());
    return;
  }

  Graph newGraph;
  for (size_t i = 0; i < vertexCount; ++i) {
    std::string vertex;
    if (!(in >> vertex)) {
      throw std::invalid_argument("");
    }
    newGraph.addVertex(vertex);
  }
  graphs.insert(name, newGraph);
}

void dribas::merge(RobinHoodHashTable< std::string, Graph >& graphs, std::istream& in)
{
  std::string newName;
  std::string name1;
  std::string name2;
  in >> newName >> name1 >> name2;

  auto it1 = graphs.find(name1);
  auto it2 = graphs.find(name2);
  if (it1 == graphs.end() || it2 == graphs.end()) {
    throw std::invalid_argument("");
  }
  Graph newGraph((*it1).second, (*it2).second);
  graphs.insert(newName, newGraph);
}

void dribas::extract(RobinHoodHashTable< std::string, Graph >& graphs, std::istream& in)
{
  std::string name;
  std::string oldName;
  size_t count;

  in >> name;
  in >> oldName;

  auto it2 = graphs.find(oldName);
  if (it2 == graphs.end()) {
    throw std::invalid_argument("");
  }

  if (!(in >> count)) {
    throw std::invalid_argument("");
  }

  if (graphs.find(name) != graphs.end()) {
    graphs.erase(name);
  }

  RobinHoodHashTable< std::string, bool > hashName;
  for (size_t i = 0; i < count; i++) {
    std::string vertexName;
    if (!(in >> vertexName)) {
      throw std::invalid_argument("");
    }
    hashName.insert(vertexName, true);
  }

  Graph newGraph((*it2).second, hashName);
  graphs.insert(name, newGraph);
}
