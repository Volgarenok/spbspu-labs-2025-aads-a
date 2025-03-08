#ifndef LIST_MANIP_HPP
#define LIST_MANIP_HPP
#include <forward_list>
#include <string>

namespace sveshnikov
{
  using list_ull_t = std::forward_list< unsigned long long >;
  using pair_t = std::pair< std::string, list_ull_t >;
  using list_pair_t = std::forward_list< pair_t >;
  using iter_list_ull_t = list_ull_t::const_iterator;
  using iter_t = list_pair_t::const_iterator;

  size_t getSizeDataList(iter_t i);
  size_t getMaxSizeLists(const list_pair_t &list);
}

#endif
