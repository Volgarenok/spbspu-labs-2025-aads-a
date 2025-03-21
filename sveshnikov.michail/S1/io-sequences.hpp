#ifndef IO_SEQUENCES_HPP
#define IO_SEQUENCES_HPP
#include <string>
#include "fwdlist-ring.hpp"

using list_ull_t = sveshnikov::FwdList< unsigned long long >;
using pair_t = std::pair< std::string, list_ull_t >;
using list_pair_t = sveshnikov::FwdList< pair_t >;
using iter_list_ull_t = sveshnikov::ConstIterator< unsigned long long >;
using iter_t = sveshnikov::ConstIterator< pair_t >;

namespace sveshnikov
{
  void inputLists(std::istream &in, list_pair_t &list);
  std::ostream &outputNamesLists(std::ostream &out, const list_pair_t &list);
  std::ostream &outputNewLists(std::ostream &out, const list_pair_t &list);
  std::ostream &outputSumsNewLists(std::ostream &out, const list_pair_t &list);
  size_t getMaxSizeLists(const list_pair_t &list);
}

#endif
