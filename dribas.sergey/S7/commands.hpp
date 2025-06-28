#ifndef CMD_HPP
#define CMD_HPP

#include <vector>

#include "graph.hpp"
#include "robinHashTable.hpp"

namespace dribas
{
  void graph(const RobinHoodHashTable< std::string, Graph >&, std::ostream&);
  void vertexes(const RobinHoodHashTable< std::string, Graph >&, std::istream&, std::ostream&);
  void outbound(const RobinHoodHashTable< std::string, Graph >&, std::istream&, std::ostream&);
  void inbound(const RobinHoodHashTable< std::string, Graph >&, std::istream&, std::ostream&);
  void bind(RobinHoodHashTable< std::string, Graph >&, std::istream&);
  void cut(RobinHoodHashTable< std::string, Graph >&, std::istream&);
  void create(RobinHoodHashTable< std::string, Graph >&, std::istream&);
  void merge(RobinHoodHashTable< std::string, Graph >&, std::istream&);
  void extract(RobinHoodHashTable< std::string, Graph >&, std::istream&);
}

#endif
