#include "graph.hpp"
#include <hash_table/definition.hpp>
#include <vector/definition.hpp>
#include <tree/definition.hpp>
#include <fwd_list/definition.hpp>

namespace {
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

  void add_edges(maslevtsov::Graph& dist, const maslevtsov::Graph::edges_set_t& src)
  {
    for (auto i = src.cbegin(); i != src.cend(); ++i) {
      for (auto j = i->second.cbegin(); j != i->second.cend(); ++j) {
        dist.bind(i->first.first, i->first.second, *j);
      }
    }
  }

  bool check_bind_existence(const std::string& vertice1, const std::string& vertice2,
    const maslevtsov::Vector< std::string >& vertices)
  {
    bool has_vertice1 = false, has_vertice2 = false;
    for (size_t i = 0; i != vertices.size(); ++i) {
      if (vertices[i] == vertice1) {
        has_vertice1 = true;
      }
      if (vertices[i] == vertice2) {
        has_vertice2 = true;
      }
    }
    if (has_vertice1 && has_vertice2) {
      return true;
    }
    return false;
  }
}

maslevtsov::Graph::Graph():
  edges_set_()
{}

maslevtsov::Graph::Graph(const Graph& src1, const Graph& src2):
  Graph()
{
  add_edges(*this, src1.edges_set_);
  add_edges(*this, src2.edges_set_);
}

maslevtsov::Graph::Graph(const Graph& src, Vector< std::string >& vertices):
  Graph()
{
  for (auto i = src.edges_set_.cbegin(); i != src.edges_set_.cend(); ++i) {
    if (check_bind_existence(i->first.first, i->first.second, vertices)) {
      for (auto j = i->second.cbegin(); j != i->second.cend(); ++j) {
        bind(i->first.first, i->first.second, *j);
      }
    }
  }
}

maslevtsov::Tree< std::string, int > maslevtsov::Graph::get_vertices() const
{
  Tree< std::string, int > vertices;
  for (auto i = edges_set_.cbegin(); i != edges_set_.cend(); ++i) {
    vertices[i->first.first];
    vertices[i->first.second];
  }
  return vertices;
}

maslevtsov::Tree< std::string, maslevtsov::Vector< unsigned > >
  maslevtsov::Graph::get_outbound(const std::string& vertice) const
{
  Tree< std::string, Vector< unsigned > > outbound;
  for (auto i = edges_set_.cbegin(); i != edges_set_.cend(); ++i) {
    if (i->first.first == vertice && !i->second.empty()) {
      for (auto j = i->second.cbegin(); j != i->second.cend(); ++j) {
        outbound[i->first.second].push_back(*j);
      }
      sort(outbound[i->first.second]);
    }
  }
  return outbound;
}

maslevtsov::Tree< std::string, maslevtsov::Vector< unsigned > >
  maslevtsov::Graph::get_inbound(const std::string& vertice) const
{
  Tree< std::string, Vector< unsigned > > outbound;
  for (auto i = edges_set_.cbegin(); i != edges_set_.cend(); ++i) {
    if (i->first.second == vertice && !i->second.empty()) {
      for (auto j = i->second.cbegin(); j != i->second.cend(); ++j) {
        outbound[i->first.first].push_back(*j);
      }
      sort(outbound[i->first.first]);
    }
  }
  return outbound;
}

void maslevtsov::Graph::bind(const std::string& vertice1, const std::string& vertice2, unsigned weight)
{
  edges_set_[std::make_pair(vertice1, vertice2)].push_back(weight);
}

bool maslevtsov::Graph::check_vertice_existence(const std::string& vertice)
{
  for (auto it = edges_set_.begin(); it != edges_set_.end(); ++it) {
    if (it->first.first == vertice || it->first.second == vertice) {
      return true;
    }
  }
  return false;
}

void maslevtsov::Graph::add_vertice(const std::string& vertice)
{
  if (check_vertice_existence(vertice)) {
    return;
  }
  bind(vertice, vertice, 0);
}

void maslevtsov::Graph::cut(const std::string& vertice1, const std::string& vertice2, unsigned weight)
{
  auto to_cut_it = edges_set_.find(std::make_pair(vertice1, vertice2));
  if (to_cut_it == edges_set_.end()) {
    throw std::invalid_argument("non-existing edge given");
  }
  bool is_deleted = false;
  auto cur = to_cut_it->second.cbegin();
  auto next = ++to_cut_it->second.cbegin();
  if (*cur == weight) {
    to_cut_it->second.pop_front();
    return;
  }
  while (next != to_cut_it->second.cend()) {
    if (*next == weight) {
      to_cut_it->second.erase_after(cur);
      is_deleted = true;
      ++cur;
      next = cur;
      ++next;
    }
  }
  if (!is_deleted) {
    throw std::invalid_argument("non-existing weight given");
  }
}
