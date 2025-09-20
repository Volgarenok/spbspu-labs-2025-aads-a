#ifndef FUNCTIONSFORLIST_HPP
#define FUNCTIONSFORLIST_HPP

#include <list>
#include <string>
#include <ostream>
#include <limits>
#include <FwdList.hpp>

namespace shramko
{
  using NumberList = ForwardList< unsigned long long >;
  using PairList = ForwardList< std::pair< std::string, NumberList > >;

  void printNames(const PairList& lists, std::ostream& out);
  bool checkSumOverflow(int a, int b);
  void printSumResult(const ForwardList< int >& sums, bool overflow, std::ostream& out);
  void processLists(const PairList& lists, size_t maxLen, bool& overflow, std::ostream& out);
}

#endif
