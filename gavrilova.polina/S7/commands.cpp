#include "commands.hpp"
#include <sstream>
#include <string>
#include <vector>

namespace {
  void sort(std::vector< std::string >& vec)
  {
    if (vec.size() < 2) {
      return;
    }
    for (size_t i = 0; i < vec.size() - 1; ++i) {
      for (size_t j = 0; j < vec.size() - i - 1; ++j) {
        if (vec[j] > vec[j + 1]) {
          std::swap(vec[j], vec[j + 1]);
        }
      }
    }
  }

  void sort(std::vector< std::pair< std::string, unsigned int > >& vec)
  {
    if (vec.size() < 2) {
      return;
    }
    for (size_t i = 0; i < vec.size() - 1; ++i) {
      for (size_t j = 0; j < vec.size() - i - 1; ++j) {
        if (vec[j].first > vec[j + 1].first) {
          std::swap(vec[j], vec[j + 1]);
        } else if (vec[j].first == vec[j + 1].first && vec[j].second > vec[j + 1].second) {
          std::swap(vec[j], vec[j + 1]);
        }
      }
    }
  }
}

void gavrilova::graphs(std::ostream& out, const GraphsCollection& graphs)
{
  if (graphs.empty()) {
    out << "\n";
    return;
  }
  std::vector< std::string > names;
  names.reserve(graphs.size());
  for (auto it = graphs.cbegin(); it != graphs.cend(); ++it) {
    names.push_back(it->first);
  }
  sort(names);
  for (size_t i = 0; i < names.size(); ++i) {
    out << names[i] << "\n";
  }
}

void gavrilova::vertexes(std::ostream& out, std::istream& in, const GraphsCollection& graphs)
{
  std::string graph_name;
  in >> graph_name;
  auto it = graphs.find(graph_name);
  if (it == graphs.cend()) {
    throw std::invalid_argument("Graph not found");
  }
  std::vector< std::string > v_names = it->second.get_vertices();
  if (v_names.empty()) {
    out << "\n";
    return;
  }
  sort(v_names);
  for (size_t i = 0; i < v_names.size(); ++i) {
    out << v_names[i] << "\n";
  }
}

void gavrilova::outbound(std::ostream& out, std::istream& in, const GraphsCollection& graphs)
{
  std::string graph_name, vertex_name;
  if (!(in >> graph_name >> vertex_name)) {
    throw std::invalid_argument("Invalid  input");
  }
  auto it = graphs.find(graph_name);
  if (it == graphs.cend() || !it->second.has_vertex(vertex_name)) {
    throw std::invalid_argument("Graph or vertex not found");
  }
  auto edges = it->second.get_outbound_edges(vertex_name);
  if (edges.empty()) {
    out << "\n";
    return;
  }
  sort(edges);
  out << edges[0].first << " " << edges[0].second;
  for (size_t i = 1; i < edges.size(); ++i) {
    if (edges[i].first == edges[i - 1].first) {
      out << " " << edges[i].second;
    } else {
      out << "\n" << edges[i].first << " " << edges[i].second;
    }
  }
  out << "\n";
}

void gavrilova::inbound(std::ostream& out, std::istream& in, const GraphsCollection& graphs)
{
  std::string graph_name, vertex_name;
  if (!(in >> graph_name >> vertex_name)) {
    throw std::invalid_argument("Invalid  input");
  }
  auto it = graphs.find(graph_name);
  if (it == graphs.cend() || !it->second.has_vertex(vertex_name)) {
    throw std::invalid_argument("Graph or vertex not found");
  }
  auto edges = it->second.get_inbound_edges(vertex_name);
  if (edges.empty()) {
    out << "\n";
    return;
  }
  sort(edges);
  out << edges[0].first << " " << edges[0].second;
  for (size_t i = 1; i < edges.size(); ++i) {
    if (edges[i].first == edges[i - 1].first) {
      out << " " << edges[i].second;
    } else {
      out << "\n" << edges[i].first << " " << edges[i].second;
    }
  }
  out << "\n";
}

void gavrilova::bind(std::istream& in, GraphsCollection& graphs)
{
  std::string graph_name, v_from, v_to;
  unsigned int weight = 0;
  if (!(in >> graph_name >> v_from >> v_to >> weight)) {
    throw std::invalid_argument("Invalid input in bind");
  }
  auto it = graphs.find(graph_name);
  if (it == graphs.end()) {
    throw std::invalid_argument("Graph not found");
  }
  it->second.add_edge(v_from, v_to, weight);
}

void gavrilova::cut(std::istream& in, GraphsCollection& graphs)
{
  std::string graph_name, v_from, v_to;
  unsigned int weight;
  if (!(in >> graph_name >> v_from >> v_to >> weight)) {
    throw std::invalid_argument("Invalid input in bind");
  }
  auto it = graphs.find(graph_name);
  if (it == graphs.end() || !it->second.remove_edge(v_from, v_to, weight)) {
    throw std::invalid_argument("Graph not found or edge doesn't exist");
  }
}

void gavrilova::create(std::istream& in, GraphsCollection& graphs)
{
  std::string graph_name;
  in >> graph_name;
  if (graphs.find(graph_name) != graphs.cend()) {
    throw std::invalid_argument("Graph already exists");
  }
  size_t count = 0;
  if (!(in >> count)) {
    throw std::invalid_argument("Invavlid input count");
  }
  Graph new_graph;
  for (size_t i = 0; i < count; ++i) {
    std::string vertex_name;
    if (!(in >> vertex_name)) {
      throw std::invalid_argument("Missing vertex name");
    }
    new_graph.add_vertex(vertex_name);
  }
  graphs[graph_name] = new_graph;
}

void gavrilova::merge(std::istream& in, GraphsCollection& graphs)
{
  std::string new_graph_name, g1_name, g2_name;
  if (!(in >> new_graph_name >> g1_name >> g2_name)) {
    throw std::invalid_argument("Invalid input in merge");
  }

  if (graphs.find(new_graph_name) != graphs.cend()) {
    throw std::invalid_argument("New graph already exists");
  }
  auto it1 = graphs.find(g1_name);
  auto it2 = graphs.find(g2_name);
  if (it1 == graphs.cend() || it2 == graphs.cend()) {
    throw std::invalid_argument("Source graph not found");
  }

  graphs[new_graph_name] = merge_graphs(it1->second, it2->second);
}

void gavrilova::extract(std::istream& in, GraphsCollection& graphs)
{
  std::string new_graph_name, old_graph_name;
  size_t count = 0;
  if (!(in >> new_graph_name >> old_graph_name >> count)) {
    throw std::invalid_argument("Invalid input in extract");
  }

  if (graphs.find(new_graph_name) != graphs.cend()) {
    throw std::invalid_argument("New graph already exists");
  }
  auto it_old = graphs.find(old_graph_name);
  if (it_old == graphs.cend()) {
    throw std::invalid_argument("Source graph not found");
  }

  std::vector< std::string > vertices_to_extract(count);
  for (size_t i = 0; i < count; ++i) {
    if (!(in >> vertices_to_extract[i])) {
      throw std::invalid_argument("Missing vertex name");
    }
    if (!it_old->second.has_vertex(vertices_to_extract[i])) {
      std::cout << "<INVALID COMMAND>\n";
      std::string dummy;
      std::getline(in, dummy);
      return;
    }
  }

  graphs[new_graph_name] = extract_graph(it_old->second, vertices_to_extract);
}

void gavrilova::readGraphsFromFile(std::istream& in, GraphsCollection& graphs)
{
  std::string line;
  while (std::getline(in, line)) {
    if (line.empty()) {
      continue;
    }

    std::stringstream ss(line);
    std::string graph_name = "";
    size_t edge_count = 0;
    ss >> graph_name >> edge_count;

    Graph current_graph;
    for (size_t i = 0; i < edge_count; ++i) {
      if (!std::getline(in, line) || line.empty()) {
        --i;
        continue;
      }
      std::stringstream edge_ss(line);
      std::string from = "";
      std::string to = "";
      unsigned int weight = 0;
      edge_ss >> from >> to >> weight;
      current_graph.add_edge(from, to, weight);
    }
    graphs[graph_name] = current_graph;
  }
}
