#ifndef OUTPUT_LIST_HPP
#define OUTPUT_LIST_HPP

#include <istream>
#include "ConstIteratorFwd.hpp"
#include "FwdList.hpp"
#include "IteratorFwd.hpp"

namespace gavrilova {
  using ULL = unsigned long long;
  using FLPairs = FwdList< std::pair< std::string, FwdList< ULL > > >;
  std::ostream& outNames(std::ostream& out, FLPairs list);
  FwdList< ULL > outNumbers(std::ostream& out, FLPairs list, size_t maxLen, size_t n);
  std::ostream& outFwdListULL(std::ostream& out, const FwdList< ULL >& list);
}

#endif
