#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_FORWARD_LIST_COMPARISON_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_FORWARD_LIST_COMPARISON_HPP

#include "algorithm-utils.hpp"
#include "forward-list-fwd.hpp"

namespace kizhin {
  template < typename T >
  bool operator==(const ForwardList< T >& lhs, const ForwardList< T >& rhs)
  {
    return lhs.size() == rhs.size() && compare(lhs.begin(), lhs.end(), rhs.begin());
  }

  template < typename T >
  bool operator!=(const ForwardList< T >& lhs, const ForwardList< T >& rhs)
  {
    return !(lhs == rhs);
  }

  template < typename T >
  bool operator<(const ForwardList< T >& lhs, const ForwardList< T >& rhs)
  {
    return lexicographicalCompare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
  }

  template < typename T >
  bool operator>(const ForwardList< T >& lhs, const ForwardList< T >& rhs)
  {
    return rhs < lhs;
  }

  template < typename T >
  bool operator<=(const ForwardList< T >& lhs, const ForwardList< T >& rhs)
  {
    return !(lhs > rhs);
  }

  template < typename T >
  bool operator>=(const ForwardList< T >& lhs, const ForwardList< T >& rhs)
  {
    return !(lhs < rhs);
  }
}

#endif

