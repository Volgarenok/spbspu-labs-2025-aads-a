#include "bfs_commands.hpp"
#include <stack.hpp>
#include <queue.hpp>
#include <hash_table/definition.hpp>

namespace {
  using distances_t = maslevtsov::HashTable< unsigned, size_t >;
  using parents_t = maslevtsov::HashTable< unsigned, unsigned >;
  using node_neighbour_pair_t = std::pair< const unsigned, maslevtsov::Vector< unsigned > >;

  void get_bfs_from(const maslevtsov::Graph& graph, unsigned start, distances_t& distances, parents_t& parents)
  {
    distances_t distances_result;
    parents_t parents_result;
    const maslevtsov::HashTable< unsigned, maslevtsov::Vector< unsigned > >& adj_list = graph.get_adj_list();
    maslevtsov::Queue< unsigned > to_visit;
    distances_result[start] = 0;
    to_visit.push(start);
    while (!to_visit.empty()) {
      unsigned current_node = to_visit.front();
      to_visit.pop();
      auto adj_list_it = adj_list.find(current_node);
      if (adj_list_it->second.empty()) {
        continue;
      }
      maslevtsov::Vector< int > dump(adj_list_it->second.size());
      for (auto i = adj_list_it->second.begin(); i != adj_list_it->second.end(); ++i) {
        if (distances_result.find(*i) == distances_result.end()) {
          distances_result[*i] = distances_result[current_node] + 1;
          parents_result[*i] = current_node;
          to_visit.push(*i);
        }
      }
    }
    distances = std::move(distances_result);
    parents = std::move(parents_result);
  }

  distances_t::const_iterator find_max_distance(const distances_t& distances)
  {
    distances_t::const_iterator largest = distances.cbegin();
    for (auto i = ++distances.cbegin(); i != distances.cend(); ++i) {
      if (largest->second < i->second) {
        largest = i;
      }
    }
    return largest;
  }

  size_t get_max_distance(const maslevtsov::Graph& graph, const node_neighbour_pair_t& node_neighbour_pair)
  {
    distances_t distances;
    parents_t parents;
    get_bfs_from(graph, node_neighbour_pair.first, distances, parents);
    auto max_el_it = find_max_distance(distances);
    return max_el_it != distances.cend() ? max_el_it->second : 0;
  }

  void sort(maslevtsov::Vector< unsigned >& vector)
  {
    if (vector.empty()) {
      return;
    }
    for (size_t i = 0; i < vector.size() - 1; ++i) {
      for (size_t j = 0; j < vector.size() - i - 1; ++j) {
        if (vector[j] > vector[j + 1]) {
          std::swap(vector[j], vector[j + 1]);
        }
      }
    }
  }
}

void maslevtsov::traverse_breadth_first(const graphs_t& graphs, std::istream& in, std::ostream& out)
{
  std::string graph_name;
  unsigned start_node = 0;
  in >> graph_name >> start_node;
  auto gr_it = graphs.find(graph_name);
  if (gr_it == graphs.end() || gr_it->second.get_adj_list().find(start_node) == gr_it->second.get_adj_list().end()) {
    throw std::invalid_argument("non-existing graph");
  }
  distances_t distances;
  parents_t parents;
  get_bfs_from(gr_it->second, start_node, distances, parents);
  for (auto i = distances.cbegin(); i != distances.cend(); ++i) {
    out << start_node << '-' << i->first << " : " << i->second << '\n';
  }
}

void maslevtsov::get_min_path(const graphs_t& graphs, std::istream& in, std::ostream& out)
{
  std::string graph_name;
  unsigned start_node = 0, goal_node = 0;
  in >> graph_name >> start_node >> goal_node;
  auto gr_it = graphs.find(graph_name);
  if (gr_it == graphs.cend() || gr_it->second.get_adj_list().find(start_node) == gr_it->second.get_adj_list().cend()) {
    throw std::invalid_argument("non-existing graph");
  }
  distances_t distances;
  parents_t parents;
  get_bfs_from(gr_it->second, start_node, distances, parents);
  if (distances.find(goal_node) == distances.end()) {
    throw std::invalid_argument("non-existing path");
  }
  maslevtsov::Stack< unsigned > restored_path;
  unsigned current_node = goal_node;
  while (current_node != start_node) {
    restored_path.push(current_node);
    current_node = parents[current_node];
  }
  out << current_node << '-' << restored_path.top();
  restored_path.pop();
  while (!restored_path.empty()) {
    out << '-' << restored_path.top();
    restored_path.pop();
  }
  out << ' ' << distances[goal_node] << '\n';
}

void maslevtsov::get_graph_width(const graphs_t& graphs, std::istream& in, std::ostream& out)
{
  std::string graph_name;
  in >> graph_name;
  auto gr_it = graphs.find(graph_name);
  if (gr_it == graphs.cend()) {
    throw std::invalid_argument("non-existing graph");
  }
  const maslevtsov::HashTable< unsigned, maslevtsov::Vector< unsigned > >& adj_list = gr_it->second.get_adj_list();
  maslevtsov::Vector< size_t > all_distances;
  size_t max_distance = get_max_distance(gr_it->second, *adj_list.begin());
  for (auto i = ++adj_list.begin(); i != adj_list.end(); ++i) {
    size_t distance = get_max_distance(gr_it->second, *i);
    if (max_distance < distance) {
      max_distance = distance;
    }
  }
  out << max_distance << '\n';
}

void maslevtsov::get_graph_components(const graphs_t& graphs, std::istream& in, std::ostream& out)
{
  std::string graph_name;
  in >> graph_name;
  auto gr_it = graphs.find(graph_name);
  if (gr_it == graphs.cend()) {
    throw std::invalid_argument("non-existing graph");
  }
  const maslevtsov::HashTable< unsigned, maslevtsov::Vector< unsigned > >& adj_list = gr_it->second.get_adj_list();
  maslevtsov::HashTable< unsigned, bool > visited_vertices;
  maslevtsov::Vector< maslevtsov::Vector< unsigned > > all_components;
  for (auto vertice_it = adj_list.begin(); vertice_it != adj_list.end(); ++vertice_it) {
    if (visited_vertices.find(vertice_it->first) != visited_vertices.end()) {
      continue;
    }
    distances_t distances;
    parents_t parents;
    get_bfs_from(gr_it->second, vertice_it->first, distances, parents);
    maslevtsov::Vector< unsigned > component;
    for (auto parent_it = parents.begin(); parent_it != parents.end(); ++parent_it) {
      if (visited_vertices.find(parent_it->first) == visited_vertices.end()) {
        visited_vertices[parent_it->first] = true;
        component.push_back(parent_it->first);
      }
      if (visited_vertices.find(parent_it->second) == visited_vertices.end()) {
        visited_vertices[parent_it->second] = true;
        component.push_back(parent_it->second);
      }
    }
    if (component.empty() && visited_vertices.find(vertice_it->first) == visited_vertices.end()) {
      component.push_back(vertice_it->first);
      visited_vertices[vertice_it->first] = true;
    }
    sort(component);
    all_components.push_back(component);
  }
  for (auto i = all_components.begin(); i != all_components.end(); ++i) {
    out << *i->begin();
    for (auto j = ++i->begin(); j != i->end(); ++j) {
      out << '-' << *j;
    }
    out << '\n';
  }
}
