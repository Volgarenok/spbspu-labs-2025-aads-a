#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>

#include "FwdListNode.hpp"
#include "constIterator.hpp"

namespace shramko
{
  template< typename T >
  class ForwardList;

  template< typename T >
  class Iterator: public ConstIterator< T >
  {
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    using ConstIterator< T >::ConstIterator;

    Iterator& operator++()
    {
      ConstIterator< T >::operator++();
      return *this;
    }

    Iterator operator++(int)
    {
      Iterator temp = *this;
      ConstIterator< T >::operator++(0);
      return temp;
    }

    reference operator*() const
    {
      return const_cast< reference >(ConstIterator< T >::operator*());
    }

    pointer operator->() const
    {
      return const_cast< pointer >(ConstIterator< T >::operator->());
    }

  private:
    friend class ForwardList< T >;
  };
}

#endif
