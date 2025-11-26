#ifndef IO_SEQUENCES_HPP
#define IO_SEQUENCES_HPP
#include <string>
#include <list/fwdlist-ring.hpp>

namespace sveshnikov
{
  using list_pair_t = FwdList< std::pair< std::string, FwdList< unsigned long long > > >;

  void inputLists(std::istream &in, list_pair_t &list);
  std::ostream &outputNamesLists(std::ostream &out, const list_pair_t &list);
  std::ostream &outputNewLists(std::ostream &out, const list_pair_t &list);
  std::ostream &outputSumsNewLists(std::ostream &out, const list_pair_t &list);
  size_t getMaxSizeLists(const list_pair_t &list);
}

#endif
