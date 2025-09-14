#ifndef SLOT_HPP
#define SLOT_HPP

#include <cstddef>
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
      size_t psl;

      Slot();
      explicit Slot(const ValueType&);
      template< class... Args >
      explicit Slot(Args&&...);
    };

    template< class K, class V >
    Slot< K, V >::Slot():
      data(ValueType{}),
      occupied(false),
      psl(0)
    {}

    template< class K, class V >
    Slot< K, V >::Slot(const ValueType& value):
      data(value),
      occupied(true),
      psl(0)
    {}

    template< class K, class V >
    template< class... Args >
    Slot< K, V >::Slot(Args&&... args):
      data(std::forward< Args >(args)...),
      occupied(true),
      psl(0)
    {}
  }
}

#endif

