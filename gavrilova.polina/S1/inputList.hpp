#ifndef INPUT_LIST_HPP
#define INPUT_LIST_HPP

#include <istream>
#include "ConstIteratorFwd.hpp"
#include "FwdList.hpp"
#include "IteratorFwd.hpp"

namespace gavrilova {
  using FLpairs = FwdList< std::pair< std::string, FwdList< unsigned long long > > >;
  FwdList< unsigned long long > inputNumbers(std::istream& in);
}

#endif
