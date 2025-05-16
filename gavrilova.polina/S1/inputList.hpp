#ifndef INPUT_LIST_HPP
#define INPUT_LIST_HPP

#include <istream>
#include "FwdList.hpp"
#include "IteratorFwd.hpp"
#include "ConstIteratorFwd.hpp"

namespace gavrilova
{
  using FLpairs = FwdList< std::pair< std::string, FwdList< unsigned long long > > >;
  FwdList< unsigned long long > inputNumbers(std::istream& in);
}

#endif
