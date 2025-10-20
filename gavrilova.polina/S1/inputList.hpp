#ifndef INPUT_LIST_HPP
#define INPUT_LIST_HPP

#include <istream>
#include "ConstIteratorFwd.hpp"
#include "FwdList.hpp"
#include "IteratorFwd.hpp"

namespace gavrilova {
  FwdList< unsigned long long > inputNumbers(std::istream& in);
}

#endif
