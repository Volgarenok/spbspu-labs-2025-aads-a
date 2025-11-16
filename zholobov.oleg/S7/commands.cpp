#include "commands.hpp"
#include <iostream>
#include <string>

void zholobov::inputGraphs(std::istream& in, MapOfGraphs& graphs)
{
  std::string name;
  size_t edgeCount = 0;
  while (in >> name >> edgeCount) {
    Graph currentGraph;
    for (size_t i = 0; i < edgeCount; ++i) {
      std::string from;
      std::string to;
      unsigned int weight;
      if (!(in >> from >> to >> weight)) {
        throw std::runtime_error("Invalid input format");
      }
      currentGraph.addEdge(from, to, weight);
    }
    graphs[name] = currentGraph;
  }
}

void zholobov::cmdPrintGraphsNames(std::ostream& out, const MapOfGraphs& graphs)
{
  if (graphs.empty()) {
    out << "\n";
    return;
  }
  for (auto& graph: graphs) {
    out << graph.first << "\n";
  }
}

void zholobov::cmdPrintVertexesNames(std::ostream& out, std::istream& in, const MapOfGraphs& graphs)
{
  std::string graphName;
  if (!(in >> graphName)) {
    throw std::invalid_argument("cmdPrintVertexesNames failed");
  }
  auto it = graphs.find(graphName);
  if (it == graphs.cend()) {
    throw std::invalid_argument("Invalid command");
  }
  auto vertices = it->second.getVertices();
  if (vertices.empty()) {
    out << "\n";
    return;
  }
  for (auto& vertex: vertices) {
    out << vertex << "\n";
  }
}

void zholobov::cmdPrintOutbounds(std::ostream& out, std::istream& in, const MapOfGraphs& graphs)
{
  std::string graphName;
  std::string vertex;
  if (!(in >> graphName >> vertex)) {
    throw std::invalid_argument("cmdPrintOutbounds failed");
  }
  auto it = graphs.find(graphName);
  if (it == graphs.cend() || !it->second.hasVertex(vertex)) {
    throw std::invalid_argument("Invalid command");
  }
  auto outbound = it->second.getOutbound(vertex);
  if (outbound.size() == 0) {
    out << "\n";
    return;
  }
  for (auto& elem: outbound) {
    out << elem.first;
    for (auto& weight: elem.second) {
      out << " " << weight;
    }
    out << "\n";
  }
}

void zholobov::cmdPrintInbounds(std::ostream& out, std::istream& in, const MapOfGraphs& graphs)
{
  std::string graphName;
  std::string vertex;
  if (!(in >> graphName >> vertex)) {
    throw std::invalid_argument("cmdPrintInbounds failed");
  }
  auto it = graphs.find(graphName);
  if (it == graphs.cend() || !it->second.hasVertex(vertex)) {
    throw std::invalid_argument("Invalid command");
  }
  auto inbound = it->second.getInbound(vertex);
  if (inbound.size() == 0) {
    out << "\n";
    return;
  }
  for (auto& elem: inbound) {
    out << elem.first;
    for (auto& weight: elem.second) {
      out << " " << weight;
    }
    out << "\n";
  }
}

void zholobov::cmdBind(std::istream& in, MapOfGraphs& graphs)
{
  std::string graphName;
  std::string from;
  std::string to;
  Graph::Weight weight;
  if (!(in >> graphName >> from >> to >> weight)) {
    throw std::invalid_argument("cmdBind failed");
  }
  auto it = graphs.find(graphName);
  if (it == graphs.end()) {
    throw std::invalid_argument("Invalid command");
  }
  it->second.addEdge(from, to, weight);
}

void zholobov::cmdCut(std::istream& in, MapOfGraphs& graphs)
{
  std::string graphName;
  std::string from;
  std::string to;
  Graph::Weight weight;
  if (!(in >> graphName >> from >> to >> weight)) {
    throw std::invalid_argument("cmdCut failed");
  }
  auto it = graphs.find(graphName);
  if (it == graphs.end() || !it->second.hasEdge(from, to, weight)) {
    throw std::invalid_argument("Invalid command");
  }
  it->second.removeEdge(from, to, weight);
}

void zholobov::cmdCreate(std::istream& in, MapOfGraphs& graphs)
{
  std::string graphName;
  if (!(in >> graphName)) {
    throw std::invalid_argument("cmdCreate failed (graph name)");
  }
  if (graphs.find(graphName) != graphs.end()) {
    throw std::invalid_argument("error");
  }
  size_t vertexCount = 0;
  if (!(in >> vertexCount)) {
    throw std::invalid_argument("cmdBind failed (number of vertexes)");
  }
  Graph newGraph;
  for (size_t i = 0; i < vertexCount; ++i) {
    std::string vertex;
    if (!(in >> vertex)) {
      throw std::invalid_argument("cmdBind failed (vertex name)");
    }
    newGraph.addVertex(vertex);
  }
  graphs[graphName] = std::move(newGraph);
}

void zholobov::cmdMerge(std::istream& in, MapOfGraphs& graphs)
{
  std::string newName;
  std::string name1;
  std::string name2;
  if (!(in >> newName >> name1 >> name2)) {
    throw std::invalid_argument("cmdMerge failed");
  }
  if (graphs.find(newName) != graphs.end()) {
    throw std::invalid_argument("new graph exists");
  }
  if (graphs.find(name1) == graphs.end() || graphs.find(name2) == graphs.end()) {
    throw std::invalid_argument("one of source graph not exist");
  }
  const Graph& g1 = graphs.at(name1);
  const Graph& g2 = graphs.at(name2);
  Graph mergedGraph;
  auto vertices1 = g1.getVertices();
  for (auto vit = vertices1.begin(); vit != vertices1.end(); ++vit) {
    mergedGraph.addVertex(*vit);
  }
  auto edges1 = g1.getAllEdges();
  for (auto eit = edges1.begin(); eit != edges1.end(); ++eit) {
    const auto& edge = eit->first;
    const auto& weights = eit->second;
    for (auto wit = weights.begin(); wit != weights.end(); ++wit) {
      mergedGraph.addEdge(edge.first, edge.second, *wit);
    }
  }
  auto vertices2 = g2.getVertices();
  for (auto vit = vertices2.begin(); vit != vertices2.end(); ++vit) {
    mergedGraph.addVertex(*vit);
  }
  auto edges2 = g2.getAllEdges();
  for (auto eit = edges2.begin(); eit != edges2.end(); ++eit) {
    const auto& edge = eit->first;
    const auto& weights = eit->second;
    for (auto wit = weights.begin(); wit != weights.end(); ++wit) {
      mergedGraph.addEdge(edge.first, edge.second, *wit);
    }
  }
  graphs[newName] = mergedGraph;
}

void zholobov::cmdExtract(std::istream& in, MapOfGraphs& graphs)
{
  std::string newName;
  std::string oldName;
  size_t vertexCount = 0;
  if (!(in >> newName >> oldName >> vertexCount)) {
    throw std::invalid_argument("cmdExtract failed");
  }
  if (graphs.find(newName) != graphs.end()) {
    throw std::invalid_argument("error");
  }
  if (graphs.find(oldName) == graphs.end()) {
    throw std::invalid_argument("error");
  }
  const Graph& source = graphs.at(oldName);
  Graph extractedGraph;
  Graph::Vertices vertices;
  for (size_t i = 0; i < vertexCount; ++i) {
    std::string vertex;
    if (!(in >> vertex)) {
      throw std::invalid_argument("error");
    }
    if (!source.hasVertex(vertex)) {
      throw std::invalid_argument("error");
    }
    vertices.push_back(vertex);
    extractedGraph.addVertex(vertex);
  }
  auto allEdges = source.getAllEdges();
  for (auto eit = allEdges.begin(); eit != allEdges.end(); ++eit) {
    const auto& edge = eit->first;
    const auto& weights = eit->second;
    bool hasFrom = false;
    bool hasTo = false;
    for (auto vit = vertices.begin(); vit != vertices.end(); ++vit) {
      if (*vit == edge.first) {
        hasFrom = true;
      }
      if (*vit == edge.second) {
        hasTo = true;
      }
    }
    if (hasFrom && hasTo) {
      for (auto wit = weights.begin(); wit != weights.end(); ++wit) {
        extractedGraph.addEdge(edge.first, edge.second, *wit);
      }
    }
  }
  graphs[newName] = extractedGraph;
}
