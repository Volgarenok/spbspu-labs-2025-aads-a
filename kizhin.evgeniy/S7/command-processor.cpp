#include "command-processor.hpp"
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <stdexcept>

namespace kizhin {
  void outGraphs(const GraphContainer&, std::ostream&);
  void outVertexes(const GraphContainer&, std::ostream&, std::istream&);
  void outOutbound(const GraphContainer&, std::ostream&, std::istream&);
  void outInbound(const GraphContainer&, std::ostream&, std::istream&);
  void bindEdge(GraphContainer&, std::istream&);
  void cutEdge(GraphContainer&, std::istream&);
  void create(GraphContainer&, std::istream&);
  void merge(GraphContainer&, std::istream&);
  void extract(GraphContainer&, std::istream&);
}

void kizhin::processCommands(GraphContainer& graphs, std::istream& in, std::ostream& out)
{
  using CmdContainer = std::map< std::string, std::function< void(void) > >;
  const auto inRef = std::ref(in);
  const auto outRef = std::ref(out);
  const CmdContainer commands = {
    { "graphs", std::bind(outGraphs, std::cref(graphs), outRef) },
    { "vertexes", std::bind(outVertexes, std::cref(graphs), outRef, inRef) },
    { "outbound", std::bind(outOutbound, std::cref(graphs), outRef, inRef) },
    { "inbound", std::bind(outInbound, std::cref(graphs), outRef, inRef) },
    { "bind", std::bind(bindEdge, std::ref(graphs), inRef) },
    { "cut", std::bind(cutEdge, std::ref(graphs), inRef) },
    { "create", std::bind(create, std::ref(graphs), inRef) },
    { "merge", std::bind(merge, std::ref(graphs), inRef) },
    { "extract", std::bind(extract, std::ref(graphs), inRef) },
  };
  CmdContainer::key_type currKey;
  while (in >> currKey) {
    try {
      commands.at(currKey)();
    } catch (const std::logic_error&) {
      out << "<INVALID COMMAND>\n";
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      in.clear();
    }
  }
}

void kizhin::outGraphs(const GraphContainer& graphs, std::ostream& out)
{
  if (graphs.empty()) {
    out << '\n';
  }
  for (const auto& graph: graphs) {
    out << graph.first << '\n';
  }
}

void kizhin::outVertexes(const GraphContainer& graphs, std::ostream& out,
    std::istream& in)
{
  GraphContainer::key_type graphName{};
  if (!(in >> graphName)) {
    throw std::logic_error("Falied to input key");
  }
  VertexContainer vertexes = getVertices(graphs.at(graphName));
  if (vertexes.empty()) {
    out << '\n';
  }
  for (const Vertex& vertex: vertexes) {
    out << vertex << '\n';
  }
}

void kizhin::outOutbound(const GraphContainer& graphs, std::ostream& out,
    std::istream& in)
{
  GraphContainer::key_type graphName{};
  Vertex vertex{};
  if (!(in >> graphName >> vertex)) {
    throw std::logic_error("Falied to input key");
  }
  const Graph& graph = graphs.at(graphName);
  if (graph.empty()) {
    throw std::logic_error("Empty graph");
  }
  SortedGraph outbounds = getOutbound(graph, vertex);
  if (outbounds.empty()) {
    out << '\n';
  }
  for (const auto& edge: outbounds) {
    const VertexPair& vertexes = edge.first;
    const WeightContiner& weights = edge.second;
    out << vertexes.to;
    for (const auto& weight: weights) {
      out << ' ' << weight;
    }
    out << '\n';
  }
}

void kizhin::outInbound(const GraphContainer& graphs, std::ostream& out, std::istream& in)
{
  GraphContainer::key_type graphName{};
  Vertex vertex{};
  if (!(in >> graphName >> vertex)) {
    throw std::logic_error("Falied to input key");
  }
  const Graph& graph = graphs.at(graphName);
  if (graph.empty()) {
    throw std::logic_error("Empty graph");
  }
  SortedGraph inbounds = getInbound(graph, vertex);
  if (inbounds.empty()) {
    out << '\n';
  }
  for (const auto& edge: inbounds) {
    const VertexPair& vertexes = edge.first;
    const WeightContiner& weights = edge.second;
    out << vertexes.from;
    for (const auto& weight: weights) {
      out << ' ' << weight;
    }
    out << '\n';
  }
}

void kizhin::bindEdge(GraphContainer& graphs, std::istream& in)
{
  GraphContainer::key_type graphName{};
  Vertex from{};
  Vertex to{};
  Weight weight{};
  if (!(in >> graphName >> from >> to >> weight)) {
    throw std::logic_error("Falied to input key");
  }
  Graph& target = graphs.at(graphName);
  target[{ from, to }].insert(weight);
}

void kizhin::cutEdge(GraphContainer& graphs, std::istream& in)
{
  GraphContainer::key_type graphName{};
  Vertex from{};
  Vertex to{};
  Weight weight{};
  if (!(in >> graphName >> from >> to >> weight)) {
    throw std::logic_error("Falied to input key");
  }
  Graph& graph = graphs.at(graphName);
  auto edgePos = graph.find({ from, to });
  if (edgePos == graph.end()) {
    throw std::logic_error("Failed to find edge");
  }
  WeightContiner& weights = edgePos->second;
  auto weightPos = weights.find(weight);
  if (weightPos == weights.end()) {
    throw std::logic_error("Failed to find weight");
  }
  weights.erase(weightPos);
  if (weights.empty()) {
    graph.erase(edgePos);
  }
  if (graph.empty()) {
    graphs.erase(graphName);
  }
}

void kizhin::create(GraphContainer& graphs, std::istream& in)
{
  GraphContainer::key_type graphName{};
  if (!(in >> graphName) || graphs.count(graphName)) {
    throw std::logic_error("Failed to perform graph creation");
  }
  Graph graph{};
  std::size_t vertexCount{};
  if (!(in >> vertexCount)) {
    throw std::logic_error("Failed to input vertex count");
  }
  Vertex current{};
  while (graph.size() != vertexCount && (in >> current)) {
    graph[{ current, current }];
  }
  if (graph.size() != vertexCount) {
    throw std::logic_error("Failed to input vertexes");
  }
  graphs.insert({ std::move(graphName), std::move(graph) });
}

void kizhin::merge(GraphContainer& graphs, std::istream& in)
{
  GraphContainer::key_type dest{};
  GraphContainer::key_type firstSrc{};
  GraphContainer::key_type secondSrc{};
  if (!(in >> dest >> firstSrc >> secondSrc)) {
    throw std::logic_error("Falied to input key");
  }
  if (!graphs.count(firstSrc) || !graphs.count(secondSrc) || graphs.count(dest)) {
    throw std::logic_error("Falied to perform merge");
  }
  Graph result = graphs[firstSrc];
  for (const auto& edge: graphs[secondSrc]) {
    const VertexPair& vertexes = edge.first;
    const WeightContiner& weights = edge.second;
    result[vertexes].insert(weights.begin(), weights.end());
  }
  result.insert(graphs[secondSrc].begin(), graphs[secondSrc].end());
  graphs[dest] = std::move(result);
}

void kizhin::extract(GraphContainer& graphs, std::istream& in)
{
  GraphContainer::key_type dest{};
  GraphContainer::key_type srcName{};
  if (!(in >> dest >> srcName) || !graphs.count(srcName) || graphs.count(dest)) {
    throw std::logic_error("Falied to perform extraction");
  }
  std::size_t vertexCount{};
  if (!(in >> vertexCount)) {
    throw std::logic_error("Failed to input vertex count");
  }
  VertexContainer vertexes{};
  Vertex current{};
  while (vertexes.size() != vertexCount && (in >> current)) {
    vertexes.insert(current);
  }
  if (vertexes.size() != vertexCount) {
    throw std::logic_error("Failed to input vertexes");
  }
  Graph result{};
  const Graph& src = graphs[srcName];
  for (const auto& edge: src) {
    const VertexPair& srcVertexes = edge.first;
    if (vertexes.count(srcVertexes.from) && vertexes.count(srcVertexes.to)) {
      result.insert(edge);
    }
  }
  graphs.insert({ std::move(dest), std::move(result) });
}

