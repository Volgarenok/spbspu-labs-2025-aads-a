#ifndef LIST_UTILS_HPP
#define LIST_UTILS_HPP

#include <iosfwd>
#include <string>
#include <list/list.hpp>

namespace maslov
{
  using list = maslov::FwdList< std::pair< std::string, maslov::FwdList< size_t > > >;
  void printData(std::ostream & out, const list & listOfPairs, size_t maxSize);
}
#endif
