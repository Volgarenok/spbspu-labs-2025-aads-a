#include "edge.hpp"

abramov::Edge::Edge():
  from(std::string{}),
  to(std::string{}),
  weight(0)
{}

abramov::Edge::Edge(const std::string &f, const std::string &t, size_t w):
  from(f),
  to(t),
  weight(w)
{}

