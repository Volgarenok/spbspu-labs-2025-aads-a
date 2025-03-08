#ifndef IO_SEQUENCES_HPP
#define IO_SEQUENCES_HPP
#include <forward_list>
#include <string>

using list_ull_t = std::forward_list< unsigned long long >;
using pair_t = std::pair< std::string, list_ull_t >;
using list_pair_t = std::forward_list< pair_t >;
using iter_list_ull_t = list_ull_t::const_iterator;
using iter_t = list_pair_t::const_iterator;

namespace sveshnikov
{
  void inputLists(std::istream &in, list_pair_t &list);
  std::ostream &outputNamesLists(std::ostream &out, const list_pair_t &list);
  std::ostream &outputNewLists(std::ostream &out, const list_pair_t &list);
  std::ostream &outputSumsNewLists(std::ostream &out, const list_pair_t &list);
}

#endif
