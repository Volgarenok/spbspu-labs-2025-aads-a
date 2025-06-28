#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_INTERNAL_FORWARD_LIST_NODE_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_INTERNAL_FORWARD_LIST_NODE_HPP

namespace kizhin {
  namespace detail {
    template < typename T >
    struct Node final
    {
      T data;
      Node* next;
    };
  }
}

#endif

