#ifndef SLOT_HPP
#define SLOT_HPP

#include <utility>

namespace aleksandrov
{
  namespace detail
  {
    template< class K, class V >
    struct Slot
    {
      using ValueType = std::pair< K, V >;

      ValueType data;
      bool occupied;
    };
  }
}

#endif

