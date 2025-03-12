#ifndef INPUT_LIST_HPP
#define INPUT_LIST_HPP

#include <istream>
#include "FwdList.hpp"

namespace gavrilova
{
  using FLpairs = FwdList< std::pair< std::string, FwdList< unsigned long long > > >;
  FwdList< unsigned long long > inputNumbers(std::istream& in, size_t& curLen);
}

#endif
