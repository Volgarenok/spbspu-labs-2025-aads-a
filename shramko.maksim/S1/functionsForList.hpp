#ifndef FUNCTIONSFORLIST_HPP
#define FUNCTIONSFORLIST_HPP

#include <cstddef>
#include <list>
#include <string>
#include "list.hpp"

namespace shramko 
{
  using list_t = std::list< unsigned long long >;
  using pairs_list_t = std::list< std::pair< std::string, list_t > >;
  std::size_t maxPairSize(pairs_list_t& list) noexcept;
  std::size_t getListElemenSum(list_t& list);
  void printInfo(std::ostream& out, pairs_list_t pairs_list);
}

#endif