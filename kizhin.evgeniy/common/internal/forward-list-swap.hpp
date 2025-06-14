#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_INTERNAL_FORWARD_LIST_SWAP_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_INTERNAL_FORWARD_LIST_SWAP_HPP

#include "forward-list-fwd.hpp"

namespace kizhin {
  template < typename T >
  void swap(ForwardList< T >& lhs, ForwardList< T >& rhs) noexcept
  {
    lhs.swap(rhs);
  }
}

#endif

