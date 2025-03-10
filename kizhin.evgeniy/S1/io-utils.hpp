#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S1_IO_UTILS_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S1_IO_UTILS_HPP

#include <iostream>
#include <forward-list.hpp>
#include "type-utils.hpp"

namespace kizhin {
  std::istream& inputSequences(std::istream&, NamesT&, ForwardList< NumbersT >&);
  template < typename T >
  std::ostream& outputList(std::ostream&, const ForwardList< T >&);
}

template < typename T >
std::ostream& kizhin::outputList(std::ostream& out, const ForwardList< T >& list)
{
  if (list.empty()) {
    return out;
  }
  out << list.front();
  for (auto i = ++list.begin(), end = list.end(); i != end; ++i) {
    out << ' ' << *i;
  }
  return out;
}

#endif

