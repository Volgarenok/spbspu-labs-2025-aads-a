#include "io-utils.hpp"
#include <iostream>

namespace kizhin {
  std::istream& operator>>(std::istream&, Graph&);
}

std::istream& kizhin::operator>>(std::istream& in, GraphContainer& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  GraphContainer input{};
  while (in) {
    GraphContainer::key_type graphName{};
    GraphContainer::mapped_type graph{};
    if (in >> graphName >> graph) {
      input.emplace(std::move(graphName), std::move(graph));
    }
  }
  in.clear();
  dest = std::move(input);
  return in;
}

std::istream& kizhin::operator>>(std::istream& in, Graph& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  Graph input{};
  std::size_t count{};
  in >> count;
  while (in && count != input.size()) {
    Vertex from{};
    Vertex to{};
    Weight weight{};
    if (in >> from >> to >> weight) {
      input[{ std::move(from), std::move(to) }].insert(weight);
    }
  }
  if (input.size() == count) {
    in.clear();
    dest = std::move(input);
  }
  return in;
}
