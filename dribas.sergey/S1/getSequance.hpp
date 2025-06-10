#ifndef GETSEQUANCE_HPP
#define GETSEQUANCE_HPP

#include <utility>
#include <string>
#include "list.hpp"

namespace dribas
{
  List< std::pair< std::string, List< unsigned long long > > > getSequance(std::istream&);
}

#endif
