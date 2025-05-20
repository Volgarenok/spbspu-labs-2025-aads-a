#ifndef OUTSEQUANCE_HPP
#define OUTSEQUANCE_HPP

#include <utility>
#include <string>
#include "list.hpp"

namespace dribas
{
  void outSequanceName(std::ostream&, const List< std::pair< std::string, List< unsigned long long > > >&);
  void outSequanceNameSum(std::ostream&, const List< std::pair< std::string, List< unsigned long long > > >&, bool&);
}

#endif
